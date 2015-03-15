import random
import pd
import sys
import ntrace

class Node:
    def __init__(self, key, value): 
	self.left = None
	self.right = None
	self.key = key
	self.value = value

def _insertNodeRec(node, newNode):
    if newNode.key < node.key:
	if node.left != None:
	    return _insertNodeRec( node.left, newNode )
	else:
	    node.left = newNode
	    return True		
    elif newNode.key > node.key:
	if node.right != None:
	    return _insertNodeRec( node.right, newNode )
	else:
	    node.right = newNode
	    return True 	   
    else: # two nodes with the same key;
	print 'two nodes with the same key: " + key + " no insert' 
	return False 
	
def _findNode(node, key):
    if node == None:
	return None
    if node.key == key:
	return node
    if key < node.key:
	return _findNode( node.left, key )
    else:
	return _findNode( node.right, key )

def _showNode(node, level):
    if node == None:
	return
    _showNode( node.left, level + 1 )
    prefix = "." * level	
    print  prefix + str( node.key )
    _showNode( node.right, level + 1 )


def _check( node ):
    if node.left != None:
	(trueL, minL, maxL) = _check( node.left )
	if not trueL or maxL >= node.key: 
	    return (False, 0, 0)
    else:
	minL = node.key 

    if node.right != None:	
	(trueR, minR, maxR) = _check( node.right )
	if not trueR or node.key >= minR:
	    return (False, 0, 0)
    else:
	maxR = node.key

    return (True, minL, maxR)

def _visitPreOrder(node, visit):
    visit(node)
    if node.left != None:
	_visitPreOrder(node.left, visit)
    if node.right != None:
	_visitPreOrder(node.right, visit)

def _visitPostOrder(node, visit):
    if node.left != None:
	_visitPostOrder(node.left, visit)
    if node.right != None:
	_visitPostOrder(node.right, visit)
    visit(node)
   
def _visitInOrder(node, visit):
    if node.left != None:
	_visitInOrder(node.left, visit)
    if node.right != None:
	_visitInOrder(node.right, visit)
    visit(node)

class BinaryTree:
    def __init__(self):
	self.root = None

    def insert(self, r):
	if self.root == None:
	    self.root = r
	    return True
	else:	    
	    return _insertNodeRec( self.root, r)	

    def find(self, key):
	return _findNode( self.root, key)

    def check(self):
	if self.root == None:
		return (True, -float('inf'), +float('inf'))
	return _check( self.root )

    def show(self):
	_showNode( self.root, 1 )

    def visitPreOrder(self, visit):
	if self.root != None:
	    _visitPreOrder(self.root, visit)

    def visitPostOrder(self, visit):
	    _visitPostOrder(self.root, visit)

    def visitInOrder(self, visit):
	if self.root != None:
	    _visitInOrder(self.root, visit)

def main():
    maxVal = 10
    keys = range(1, maxVal)
    random.shuffle( keys )

    print "after shuffle"

    t = BinaryTree()

    for k in keys:      
	ntrace.enabletrace( True )
	assert t.insert( Node( k, 'key: ' + str(k) ) ) == True
  	ntrace.enabletrace( False )
 
    print "after insert"

    t.show()

    (isBinTree, minKey, maxKey) = t.check()
    print "is binary tree: ", isBinTree, "min: ", minKey, "max: ", maxKey
    
    assert isBinTree == True
    assert minKey == 1 
    assert maxKey == maxVal - 1

    for k in xrange(1, maxVal):
	n = t.find( k )
	assert n != None
	sval = 'key: ' + str(k)
	assert n.value == sval

    visit = lambda node: sys.stdout.write( '[ ' + str( node.key ) + ' ' + node.value + ' ] ') 
    
    print 'visit preorder'
    t.visitPreOrder( visit )	

    print '\nvisit postorder'
    t.visitPostOrder( visit )
    
    print '\nvisit inorder'
    t.visitInOrder( visit )

    print '\n*** test completed ***'


if __name__ == "__main__":
	main()
	

