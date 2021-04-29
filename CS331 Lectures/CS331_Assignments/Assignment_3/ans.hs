import Data.List

--Question 3
--Quick sort with the first element as the partition
quickSort :: Ord a => [a] -> [a]
quickSort [] = []
quickSort (x:xs) = (quickSort ls) ++ [x] ++ (quickSort rs)
          where  ls = [y | y <- xs, y <= x] 
                 rs = [y | y <- xs, y >  x] 

--Question 1
--Newton's method for calculating square root
squareRoot :: (Ord t, Fractional t) => t -> t
squareRoot n = newton n n 0.00001

newton :: (Ord t, Fractional t) => t -> t -> t -> t
newton n root err
     | n < 0                        = error "Enter a positive number"
     | abs (root * root - n) <= err = root
     | otherwise                    = newton n newRoot err
                                       where newRoot = 0.5 * (root + (n / root))



--Qustion 2
--fibonacci O(n) calculation
fibonacci :: (Ord t, Num t, Num p) => t -> p
fibonacci n
     | n < 1     = error "Enter valid index >= 1"
     | otherwise = fibonacciHelper 1 1 n 

fibonacciHelper :: (Eq t1, Num t1, Num t2) => t2 -> t2 -> t1 -> t2
fibonacciHelper a b n
     | n == 1    = a
     | n == 2    = b
     | otherwise = fibonacciHelper b (a + b) (n - 1)

--test the function on tests
printTest :: (Show a, Show b) => (a -> b) -> [a] -> [Char]
printTest f tests = intercalate "\n" (map show (zip input output))
              where input  = map id tests
                    output = map f tests 

--main function
main :: IO()
main = do
      -- tests
      let fibonacci_tests = [1, 4, 10, 20, 30, 40, 100, 200, 500]
      let squareRoot_tests = [0, 1, 4, 0.36, 23.56, 100, 1024, 233421]
      let quickSort_tests = [[], [1, 3, 2], [5, 4, 3, 2, 1], [1, 2, 3, 4, 5], [100, 234, 12, 124, 1, 22, 73]]
      -- testing
      putStrLn("Fibonacci (input, output)")
      putStrLn(printTest fibonacci fibonacci_tests)
      putStrLn("SquareRoot (input, output)")
      putStrLn(printTest squareRoot squareRoot_tests)
      putStrLn("QuickSort (input, output)")
      putStrLn(printTest quickSort quickSort_tests)
      return ()
