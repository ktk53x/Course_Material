import Data.List.Split

--Binary Tree DataStructure definition
data Tree a = Nil | Node (Tree a) a (Tree a) deriving (Show)

--check if the Tree is empty or not
isEmpty Nil = True
isEmpty _   = False

--insert an element in the BST
insert Nil key                          = Node (Nil) key (Nil)
insert (Node (left) root (right)) key   = if      key < root  then (Node (insert left key) root (right))
                                          else if key == root then (Node (left) root (right))
                                          else                     (Node (left) root (insert right key))

--search an element in the BST
search key Nil = False
search key (Node (left) root (right)) = if      key < root  then search key left
                                        else if key == root then True
                                        else                     search key right

--creates BST from a list of numbers
createBST xs = foldl (insert) Nil xs 

--inorderTraversal of the BST
inorderTraversal Nil = []
inorderTraversal (Node (left) root (right)) = (inorderTraversal(left) ++ (root: inorderTraversal(right)))

--postorderTraversal of the BST
postorderTraversal Nil = []
postorderTraversal (Node (left) root (right)) = (postorderTraversal(left) ++ postorderTraversal(right) ++ [root])

--preorderTraversal of the BST
preorderTraversal Nil = []
preorderTraversal (Node (left) root (right)) = (root: (preorderTraversal(left) ++ preorderTraversal(right)))

--return all leaves of the BST
leaves Nil = []
leaves (Node (Nil) root (Nil)) = [root]
leaves (Node (left) root (right)) = (leaves left) ++ (leaves right)

--return lcm of all numbers in the list
listLCM xs = foldl1 lcm xs



--parse the input string to list of integers
parse line = (map (read::String->Integer) (splitOn "," line))

--take list input
getList = do 
          putStrLn "Enter a list of numbers separated by commas."
          line <- getLine
          putStrLn ("Your List is: " ++ line)
          let inputList = parse(line)
          return inputList


answers = do
          --A)
          inputList <- getList
          --B)
          let listLcm = listLCM inputList
          putStrLn("LCM: " ++ show(listLcm))
          --C)
          let listBST = createBST inputList
          putStrLn("preorderTraversal: " ++ show(preorderTraversal listBST))
          putStrLn("inorderTraversal: " ++ show(inorderTraversal listBST))
          putStrLn("postorderTraversal: " ++ show(postorderTraversal listBST))
          return ()

main = do answers
