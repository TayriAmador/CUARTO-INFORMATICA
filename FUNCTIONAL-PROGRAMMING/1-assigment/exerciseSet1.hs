{- 
 * Vilnius University
 * Functional Programming
 * Excercises set 1
 * @author Tayri Amador Guillén
 * @date 2026-10-06
 * @brief This is the solution for the exercises set 1
-}
module ExerciseSet1 where

-- Excercise 1 v1 -- 
nAnd1 :: Bool -> Bool -> Bool
nAnd1 x y = not (x && y)

-- v2 --
nAnd2 :: Bool -> Bool -> Bool
nAnd2 True y  = not y
nAnd2 False y = True

-- v3:--  
nAnd3 :: Bool -> Bool -> Bool
nAnd3 True True   = False
nAnd3 True False  = True
nAnd3 False True  = True
nAnd3 False False = True

-- v4 --
nAnd4 :: Bool -> Bool -> Bool
nAnd4 True True = False
nAnd4 _ _ = True

-- Exercise 2 --
nDigits :: Integer -> Int
nDigits n
  | n < 0 = length(show n) - 1
  | otherwise = length (show n) --show converts n into a string--

-- Exercise 3 --
nRoots :: Float -> Float -> Float -> Int
nRoots a b c
  | a == 0.0 = error "the first argument should be non-zero!"
  | b*b > 4.0 * a * c = 2
  | b*b == 4.0 * a * c = 1
  | otherwise = 0

-- Exercise 4 --
smallerRoot :: Float -> Float -> Float -> Float
smallerRoot a b c
  | nRoots a b c == 0 = error "no real roots" 
  | nRoots a b c == 1 = -b / (2.0 * a)
  | otherwise         = min root1 root2
  where
    root1        = (-b + sqrt(b^2 - 4.0 * a * c)) / (2.0 * a)
    root2        = (-b - sqrt (b^2 - 4.0 * a * c)) / (2.0 * a)

largerRoot :: Float -> Float -> Float -> Float
largerRoot a b c
  | nRoots a b c == 0 = error "no real roots"
  | nRoots a b c == 1 = -b / (2.0 * a)
  | otherwise         = max root1 root2
  where
    root1        = (-b + sqrt(b^2 - 4.0 * a * c)) / (2.0 * a)
    root2        = (-b - sqrt(b^2 - 4.0 * a * c)) / (2.0 * a)


-- Exercise 5 --
power2 :: Integer -> Integer 
power2 0 = 1
power2 n 
  | n < 0 = 0
  | otherwise = 2 * power2(n-1)

-- Exercise 6 --
mult :: Integer -> Integer -> Integer 
mult 0 _ = 0 
mult _ 0 = 0 
mult 1 m = m
mult n 1 = n

mult n m
  | n > 0 = m + mult(n - 1) m
  | otherwise = mult (-n) (-m) 

-- Exercise 7 --
prod :: Integer -> Integer -> Integer
prod m n
  | m > n      = error "Invalid range"
  | (n - m) == 0 = m
  | (n - m) > 0  = n * prod m (n - 1)

fact :: Integer -> Integer
fact 0 = 1
fact n = prod 1 n