import numpy as np
 
epochs = 10000                                  # Number of iterations
inputLayerSize, hiddenLayerSize, outputLayerSize = 2, 3, 1
L = 0.1                                         # learning rate      
 
X = np.array([[0,0], [0,1], [1,0], [1,1]])      # Buttons states array
Y = np.array([ [0],   [1],   [1],   [0]])       # LED states array
 
def sigmoid (x): return 1/(1 + np.exp(-x))      # activation function
                                                # weights on layer inputs
Wh = np.random.uniform(size=(inputLayerSize, hiddenLayerSize))
Wz = np.random.uniform(size=(hiddenLayerSize,outputLayerSize))
 
for i in range(epochs):
 
    H = sigmoid(np.dot(X, Wh))                  # calculate forward part
    Z = np.dot(H,Wz)                            # 
    E = Y - Z                                   # calculate error
    dZ = E * L                                  # delta Z
    Wz +=  H.T.dot(dZ)                          # calculate backpropagation part
    dH = dZ.dot(Wz.T) * sigmoid_deriv(H)        # 
    Wh +=  X.T.dot(dH)                          # update hidden layer weights

print("**************** error ****************") 
print(E)
print("***************** output **************") 
print(Z)   
print("*************** weights ***************") 
print("input to hidden layer weights: ")     
print(Wh)
print("hidden to output layer weights: ")
print(Wz)