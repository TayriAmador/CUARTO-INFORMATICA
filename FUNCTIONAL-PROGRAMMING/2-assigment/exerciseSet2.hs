{- 
 * Vilnius University
 * Functional Programming
 * Excercises set 2
 * @author Tayri Amador Guillén
 * @date 2026-10-26
 * @brief This is the solution for the exercises set 2
-}

module ExerciseSet2 where

--- Exercise 1
average :: [Float] -> Float
average [] = 0
average numbers = sum numbers / fromIntegral(length numbers)

--- Exercise 2
--- Ex2 -> Recursion
divides :: Integer -> [Integer]
divides 0 = []
divides number 
  | number > 0 = helper number 1
  | otherwise = helper number (-abs number)
  where
    helper n i
      | i > abs n = []
      | i == 0 = helper n (i + 1)
      | n `mod` i == 0 = i : helper n (i + 1)
      | otherwise = helper n (i + 1)  

--- Ex2 -> List comprehension
dividesCom :: Integer -> [Integer]
dividesCom 0 = []
dividesCom n 
  | n > 0 = [x | x <- [1 .. div n 2], n `mod` x == 0] ++ [n]
  | otherwise = [x | x <- [-abs n .. (-1)] ++ [1 .. abs n], n `mod` x == 0]

--- Ex2 -> Prime number
isPrime :: Integer -> Bool
isPrime n
  | n < 2 = False
  | otherwise = dividesCom n == [1, n]

--- Exercise 3
prefix :: String -> String -> Bool
prefix string1 string2 
  | length string1 > length string2 = False
  | otherwise = take (length string1) string2 == string1 

--- Ex3 -> substring
substring :: String -> String -> Bool
substring [] _ = True
substring _ [] = False
substring string1 string2
  | prefix string1 string2 = True
  | otherwise = substring string1 (tail string2)

--- Exercise 4
permut :: [Integer] -> [Integer] -> Bool
permut [] [] = True
permut xs ys
  | length xs /= length ys = False
  | otherwise = all (\x -> count x xs == count x ys) xs
  where
    count n zs = length [z | z <- zs, z == n]

--- Exercise 5
capitalise :: String -> String
capitalise str = [toUpper c | c <- str, isAlpha c]
  where
    isAlpha c = ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')
    toUpper c
      | 'a' <= c && c <= 'z' = toEnum (fromEnum c - 32)
      | otherwise = c

--- Generalisation
transformString :: (Char -> Bool) -> (Char -> Char) -> String -> String
transformString condition transform str = [transform c | c <- str, condition c]

--- Exercise 6
itemTotal :: [(String, Float)] -> [(String, Float)]
itemTotal [] = []
itemTotal ((name, price): xs) = (name, totalFor name xs) : itemTotal remaining 
  where
    totalFor n items = price + sum [p | (m, p) <- items, m == n]
    remaining = [(m, p) | (m, p) <- xs, m /= name]

--- Exercise 6 -> discounts
itemDiscount :: String -> Integer -> [(String, Float)] -> [(String, Float)]
itemDiscount item disc basket = [(name, newPrice name price) | (name, price) <- basket] 
  where
    newPrice name price
      | name == item = price * (1 - fromInteger disc / 100)
      | otherwise = price