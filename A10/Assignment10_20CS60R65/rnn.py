import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)
import numpy as np
import sys
import os
import tensorflow.compat.v1 as tf
tf.disable_v2_behavior()
from sklearn.model_selection import train_test_split
import platform

class_names = ['T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat',
               'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot']

total_class = len(class_names)
# Parameters
n_inputs = 28 ### lengh of each row
n_steps = 28 ### number of time steps
n_layers = 3 ### number of BasicRNNCell layers
n_neurons = 100 ### number of neurons in the network
n_outputs = 10 ### outputs that represent digits from 0-9
training_epochs = 100
learning_rate = 0.001
batch_size = 50


def reset_graph(seed=42): ### it resests all created graph, it's required once re-defining of any placeholders, variables, shapes or model structures is needed
    tf.reset_default_graph()
    tf.set_random_seed(seed)
    np.random.seed(seed)


def generate_batch(x_train, y_train, batch_size): ### it takes random permutation of lenght x_train and splits x_train (together with y_train) into batches number
    rnd_idx = np.random.permutation(len(x_train))
    n_batches = len(x_train) // batch_size
    for batch_idx in np.array_split(rnd_idx, n_batches):
        x_batch, y_batch = x_train[batch_idx], y_train[batch_idx]
        yield x_batch, y_batch


(x_train, y_train), (x_test, y_test) = tf.keras.datasets.fashion_mnist.load_data()
x_train = x_train.astype(np.float32).reshape(-1, 28*28) / 255.0
x_test = x_test.astype(np.float32).reshape(-1, 28*28) / 255.0
y_train = y_train.astype(np.int32)
y_test = y_test.astype(np.int32)


reset_graph()


with tf.name_scope("Inputs"):
    X = tf.placeholder(tf.float32, [None, n_steps, n_inputs], name="X")
    y = tf.placeholder(tf.int32, [None], name="y")
    keep_prob = tf.placeholder_with_default(1.0, shape=(), name='keep_probability')



with tf.name_scope("Basic_RNN_Layers"):
    lstm_cells = [tf.nn.rnn_cell.BasicRNNCell(num_units = n_neurons, activation = tf.nn.relu)
             for layer in range(n_layers)]
    lstm_cells_drop = [tf.nn.rnn_cell.DropoutWrapper(cell, input_keep_prob=keep_prob)
                for cell in lstm_cells]
    multi_layer_cell = tf.nn.rnn_cell.MultiRNNCell(lstm_cells_drop)
    outputs, states = tf.nn.dynamic_rnn(multi_layer_cell, X, dtype = tf.float32)


with tf.name_scope("Loss"):
    states_concat = tf.concat(axis=1, values=states, name='states_reshape')
    dense1 = tf.layers.dense(states_concat, 64, name='dense_1')
    dense2 = tf.layers.dense(dense1, 32, name='dense_2')
    logits = tf.layers.dense(dense2, n_outputs, name='output_layer')
    xentropy = tf.nn.sparse_softmax_cross_entropy_with_logits(labels=y, logits=tf.reshape(logits, shape=(-1, n_outputs)), name='softmax_cross_entropy')
    loss = tf.reduce_mean(xentropy, name='loss')
    loss_summary = tf.summary.scalar('loss_summ', loss)


with tf.name_scope("Train"):    
    optimizer = tf.train.AdamOptimizer(learning_rate=0.001, name='Adam_optimizer')
    training_optimizer = optimizer.minimize(loss, name='training_Adam')


with tf.name_scope("Evaluation"):        
    correct = tf.nn.in_top_k(tf.reshape(logits, (-1, n_outputs)), y, 1, name='inTopK')
    accuracy = tf.reduce_mean(tf.cast(correct, tf.float32), name='Accuracy')
    accuracy_summary = tf.summary.scalar('Accuracy_Summ', accuracy)


init = tf.global_variables_initializer()
saver = tf.train.Saver()


train_keep_prob = 0.8

x_test = x_test.reshape((-1, n_steps, n_inputs))


from datetime import datetime

def log_dir(prefix=""):
    now = datetime.utcnow().strftime('%Y-%m-%d-%H-%m-%S')
    root_logdir = "TensorFlow_Logs"
    if prefix:
        prefix += '-'
    name = prefix + now
    return '{}/{}/'.format(root_logdir, name)

logdir=log_dir("mnist_rnn_model")
file_writer = tf.summary.FileWriter(logdir, tf.get_default_graph())


checkpoint_path = "./checkpoints/mnist_rnn_model.ckpt"
checkpoint_epoch_path = checkpoint_path + ".epoch"
final_model_path = "./mnist_rnn_model"



def train():

    best_loss = np.infty
    epochs_without_progress = 0
    max_epochs_without_progress = 15


    acc_list, acc_test_list, loss_list, loss_test_list = [], [], [], []
    n_epochs = 400
    batch_size = 128
    with tf.Session() as sess:
        init.run() 
        for epoch in range(n_epochs):
            for x_batch, y_batch in generate_batch(x_train, y_train, batch_size):
                x_batch = x_batch.reshape((-1, n_steps, n_inputs))

                sess.run(training_optimizer, feed_dict={X: x_batch, y: y_batch, keep_prob: train_keep_prob})
            acc_batch, loss_batch, acc_sum, loss_sum = sess.run([accuracy, loss, accuracy_summary, loss_summary], feed_dict={X: x_batch, y: y_batch, keep_prob: train_keep_prob})   
   
            acc_test, loss_test, acc_test_sum, loss_test_sum = sess.run([accuracy, loss, accuracy_summary, loss_summary], feed_dict={X: x_test, y: y_test})

            acc_list.append(acc_batch)
            loss_list.append(loss_batch)
            acc_test_list.append(acc_test)
            loss_test_list.append(loss_test)

            file_writer.add_summary(acc_sum, epoch)
            file_writer.add_summary(loss_sum, epoch)
            file_writer.add_summary(acc_test_sum, epoch)
            file_writer.add_summary(loss_test_sum, epoch)

            if epoch % 5 == 0:
                print("Epoch", epoch,
                      '\tValidation accuracy: {:.3f}%'.format(acc_batch * 100), '\tLoss: {:.3f}'.format(loss_batch))
                saver.save(sess, checkpoint_path)
                with open(checkpoint_epoch_path, "wb") as f:
                    f.write(b'%d' % (epoch + 1))
                if loss_batch < best_loss:
                    saver.save(sess, final_model_path)
                    best_loss = loss_batch
                else:
                    epochs_without_progress += 2
                    if epochs_without_progress > max_epochs_without_progress:
                        print('Early Stopping')
                        break


def test():
    best_loss = np.infty
    epochs_without_progress = 0
    max_epochs_without_progress = 15


    acc_list, acc_test_list, loss_list, loss_test_list = [], [], [], []
    n_epochs = 400
    batch_size = 128
    with tf.Session() as sess:
        init.run() 
        for epoch in range(n_epochs):
            for x_batch, y_batch in generate_batch(x_train, y_train, batch_size):
                x_batch = x_batch.reshape((-1, n_steps, n_inputs))

                sess.run(training_optimizer, feed_dict={X: x_batch, y: y_batch, keep_prob: train_keep_prob})
            acc_batch, loss_batch, acc_sum, loss_sum = sess.run([accuracy, loss, accuracy_summary, loss_summary], feed_dict={X: x_batch, y: y_batch, keep_prob: train_keep_prob})   
   
            acc_test, loss_test, acc_test_sum, loss_test_sum = sess.run([accuracy, loss, accuracy_summary, loss_summary], feed_dict={X: x_test, y: y_test})

            acc_list.append(acc_batch)
            loss_list.append(loss_batch)
            acc_test_list.append(acc_test)
            loss_test_list.append(loss_test)  

            file_writer.add_summary(acc_sum, epoch)
            file_writer.add_summary(loss_sum, epoch)
            file_writer.add_summary(acc_test_sum, epoch)
            file_writer.add_summary(loss_test_sum, epoch)

            if epoch % 5 == 0:
                print("Epoch", epoch, '\tTest accuracy: {:.3f}%'.format(acc_test * 100), '\tLoss: {:.3f}'.format(loss_batch))
                saver.save(sess, checkpoint_path)
                with open(checkpoint_epoch_path, "wb") as f:
                    f.write(b'%d' % (epoch + 1))
                if loss_batch < best_loss:
                    saver.save(sess, final_model_path)
                    best_loss = loss_batch
                else:
                    epochs_without_progress += 2
                    if epochs_without_progress > max_epochs_without_progress:
                        print('Early Stopping')
                        break


def test2():
    print("+" * 80)
    print("Testing Phase")
    print("+" * 80)
    import mnist_reader
    X_test, y_test = mnist_reader.load_mnist('../data/fashion', kind='t10k')
    X_test = -0.5 + (X_test / 255.0)
    y_test = np.eye(total_class)[y_test]
    print("Length of Training Data:", len(X_test))
    tf.compat.v1.reset_default_graph()
    saver = tf.compat.v1.train.import_meta_graph("weight/model.ckpt.meta")
    x = tf.compat.v1.placeholder(tf.float64, [None, 784])
    y_ = tf.compat.v1.placeholder(tf.float64, [None, 10])
    with tf.compat.v1.Session() as sess:
        saver.restore(sess, tf.train.latest_checkpoint("weight/"))
        graph = tf.compat.v1.get_default_graph()
        w1 = graph.get_tensor_by_name("w1:0")
        b1 = graph.get_tensor_by_name("b1:0")
        w2 = graph.get_tensor_by_name("w2:0")
        b2 = graph.get_tensor_by_name("b2:0")
        w3 = graph.get_tensor_by_name("w3:0")
        b3 = graph.get_tensor_by_name("b3:0")
        w4 = graph.get_tensor_by_name("w4:0")
        b4 = graph.get_tensor_by_name("b4:0")
        h1 = tf.add(tf.matmul(x, w1), b1)
        h1 = reluActivation(h1)
        h2 = tf.add(tf.matmul(h1, w2), b2)
        h2 = reluActivation(h2)
        h3 = tf.add(tf.matmul(h2, w3), b3)
        h3 = reluActivation(h3)
        predicted = tf.add(tf.matmul(h3, w4), b4)
        predicted = tf.argmax(predicted, 1)
        actual = tf.argmax(y_, 1)
        correct_prediction = tf.equal(predicted, actual)
        accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float64))
        test_accuracy = sess.run(accuracy, feed_dict={x: X_test, y_: y_test})
        print("Test Accuracy:", round(test_accuracy * 100, 3), "%")


def train2(hidden_unit_size=64):
    global learning_rate
    global patience
    print("+" * 80)
    print("Training Phase")
    print("+" * 80)
    import mnist_reader
    X_train, y_train = mnist_reader.load_mnist('../data/fashion', kind='train')
    X_train, X_valid, y_train, y_valid = train_test_split(
        X_train, y_train, test_size=(1 / 6), random_state=48)
    # Normalize input
    X_train = -0.5 + (X_train / 255.0)
    X_valid = -0.5 + (X_valid / 255.0)
    # One Hot Encoding for Training and Validation Set
    y_train = np.eye(total_class)[y_train]
    y_valid = np.eye(total_class)[y_valid]
    print("Length of Training Data:", len(X_train))
    print("Length of Validation Data", len(X_valid))
    # Logits
    x = tf.compat.v1.placeholder(tf.float64, [None, 784])
    w1 = tf.Variable(tf.random.normal(
        [784, hidden_unit_size], mean=0.0, stddev=np.math.sqrt(2 / (hidden_unit_size)), dtype=tf.float64), name="w1")
    b1 = tf.Variable(tf.zeros([hidden_unit_size],
                              dtype=tf.float64),  name="b1")
    layer1 = tf.add(tf.matmul(x, w1), b1)
    layer1 = reluActivation(layer1)
    w2 = tf.Variable(tf.random.normal(
        [hidden_unit_size, hidden_unit_size], mean=0.0, stddev=np.math.sqrt(2 / (hidden_unit_size)), dtype=tf.float64), name="w2")
    b2 = tf.Variable(tf.zeros(
        [hidden_unit_size], dtype=tf.float64), name="b2")
    layer2 = tf.add(tf.matmul(layer1, w2), b2)
    layer2 = reluActivation(layer2)
    w3 = tf.Variable(tf.random.normal(
        [hidden_unit_size, hidden_unit_size],  mean=0.0, stddev=np.math.sqrt(2 / (hidden_unit_size)), dtype=tf.float64), name="w3")
    b3 = tf.Variable(tf.zeros([hidden_unit_size], dtype=tf.float64), name="b3")
    layer3 = tf.add(tf.matmul(layer2, w3), b3)
    layer3 = reluActivation(layer3)
    w4 = tf.Variable(tf.random.normal(
        [hidden_unit_size, 10],  mean=0.0, stddev=np.math.sqrt(2 / (hidden_unit_size)), dtype=tf.float64), name="w4")
    b4 = tf.Variable(tf.zeros([10], dtype=tf.float64), name="b4")
    layer4 = tf.add(tf.matmul(layer3, w4), b4)
    y_ = tf.compat.v1.placeholder(tf.float64, [None, 10])
    output = softMax(layer4)
    # loss calculation
    avgerageLoss = tf.reduce_mean(tf.compat.v1.losses.softmax_cross_entropy(y_, layer4)) + 0.0002 * (tf.reduce_sum(tf.multiply(
        w1, w1)) + tf.reduce_sum(tf.multiply(w2, w2)) + tf.reduce_sum(tf.multiply(w3, w3)) + tf.reduce_sum(tf.multiply(w4, w4)))
    # Minimize Cost
    optimizer_adam = tf.compat.v1.train.AdamOptimizer(
        learning_rate=learning_rate).minimize(avgerageLoss)
    correct_prediction = tf.equal(tf.argmax(output, 1), tf.argmax(y_, 1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float64))
    validationLoss = tf.reduce_mean(
        tf.compat.v1.losses.softmax_cross_entropy(y_, layer4))
    # Initialize Variable
    init = tf.compat.v1.global_variables_initializer()
    saver = tf.compat.v1.train.Saver()
    with tf.compat.v1.Session() as sess:
        sess.run(init)
        best_val = 0
        best_val_e = -1
        for e in range(training_epochs):
            print("Epoch", e + 1)
            epoch_loss = 0
            for batch_index in range(0, X_train.shape[0], batch_size):
                miniBatch_X = X_train[batch_index:batch_index + batch_size, :]
                miniBatch_Y = y_train[batch_index:batch_index + batch_size, :]
                _, batch_loss = sess.run([optimizer_adam, avgerageLoss], feed_dict={
                    x: miniBatch_X, y_: miniBatch_Y})
                epoch_loss += batch_loss
            epoch_loss = epoch_loss / (X_train.shape[0] / batch_size)
            print("Train_loss: ", epoch_loss, end=" ")
            trainAccuracy = sess.run(accuracy, feed_dict={
                x: X_train, y_: y_train})
            print("Train_accuracy:", round(float(trainAccuracy * 100), 3), "%")
            validLoss, validAccuracy = sess.run([validationLoss, accuracy], feed_dict={
                x: X_valid, y_: y_valid})
            print("Validation loss:", validLoss, end=" ")
            print("Validation Accuracy:", round(validAccuracy * 100, 3), "%")
            if(best_val < validAccuracy):
                best_val = validAccuracy
                best_val_e = e
                save_path = saver.save(sess, "weight/model.ckpt")
            elif(best_val_e + patience <= e):
                print("Early Stopping.\nUsing Epoch", best_val_e + 1)
                print("Validation Accuracy", round(best_val * 100, 3), "%")
                break



print("*" * 80)
print("*" * 80)
if(len(sys.argv) != 2):
    print("Invalid number of command line arrgument provided")
    print("--train :to Train the model")
    print("--test  :to Test the model")
    sys.exit()
if(sys.argv[1] == "--train"):
    train()
    sys.exit()
elif(sys.argv[1] == "--test"):
    test()
    sys.exit()
else:
    print("Invalid argument provided")
    print("--train :to Train the model")
    print("--test  :to Test the model")
