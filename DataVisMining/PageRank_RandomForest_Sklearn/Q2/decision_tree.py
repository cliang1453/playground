from util import entropy, information_gain, partition_classes
import numpy as np 
import ast

class DecisionTree(object):
    def __init__(self):
        # Initializing the tree as an empty dictionary or list, as preferred
        #self.tree = []
        self.tree = {}
        self.randomness_ratio = 0.8
        self.min_random = 5

    
    def learn_helper(self, X, y, curr):

        if entropy(y) == 0:
            curr['split_attr'] = -1
            curr['split_val'] = y[0]
            return 

        if len(X[0]) == 1:
            curr['split_attr'] = -1
            curr['split_val'] = int(float(sum(y))/len(y)>0.5)
            return 


        opt = ()
        max_info_gain = 0

        # introduce some randomness
        if len(X[0]) >= self.min_random:
            num_attr_to_pick = int(len(X[0])*self.randomness_ratio)
            split_attrs = np.random.choice(np.arange(len(X[0])), num_attr_to_pick, replace=False)
        else:
            split_attrs = list(np.arange(len(X[0])))
        

        for split_attr in split_attrs:
            split_vals = set([row[split_attr] for row in X])
            for split_val in split_vals:
                split = partition_classes(X, y, split_attr, split_val)
                if len(split[2])==0 or len(split[3])==0:
                    continue
                info_gain = information_gain(y, [split[2], split[3]])
                if info_gain > max_info_gain:
                    max_info_gain = info_gain
                    opt = (split, split_attr, split_val)


        split, split_attr, split_val = opt
        curr['left'] = {}
        curr['right'] = {}
        curr['split_attr'] = split_attr
        curr['split_val'] = split_val

        self.learn_helper(split[0], split[2], curr['left'])
        self.learn_helper(split[1], split[3], curr['right'])


    def learn(self, X, y):
        # TODO: Train the decision tree (self.tree) using the the sample X and labels y
        # You will have to make use of the functions in utils.py to train the tree
        
        # One possible way of implementing the tree:
        #    Each node in self.tree could be in the form of a dictionary:
        #       https://docs.python.org/2/library/stdtypes.html#mapping-types-dict
        #    For example, a non-leaf node with two children can have a 'left' key and  a 
        #    'right' key. You can add more keys which might help in classification
        #    (eg. split attribute and split value)
        root = self.tree
        self.learn_helper(X, y, root)


    def classify_helper(self, X, curr):
        
        if curr['split_attr'] == -1:
            pred = curr['split_val']
            return pred

        if isinstance(curr['split_val'], str):
            if X[curr['split_attr']] == curr['split_val']:
                return self.classify_helper(X, curr['left'])
            else:
                return self.classify_helper(X, curr['right'])
        else:
            if X[curr['split_attr']] <= curr['split_val']:
                return self.classify_helper(X, curr['left'])
            else:
                return self.classify_helper(X, curr['right'])

    def classify(self, record):
        # TODO: classify the record using self.tree and return the predicted label
        root = self.tree
        pred = self.classify_helper(record, root)
        return pred


