module Fact where

-- Function/Operation:
-- null, head, tail, drop, take, last, init, reverse, elem !!
-- > < + - * / ++ == /= || &&
-- `prefix`
-- length, sum, product, maximum minimum, replicate

fac 0 = 1
fac n = n * fac (n-1)


t01list = [x | x <- [1..10], x `mod` 2 == 0]
t02list = [ (x, y) | x <- [1..10], y <- [1..10], x `mod` 2 == 0, y `mod` 2 == 0]
t03list = [x | x <- [1..10], x /= 1, x /= 2, x /= 4]

t04list = [1..5] !! 2

f01func xs = [ if x > 4 then 'B' else 'A' | x <- xs ] 
f02func c1 c2 = c1 ++ c2
f03func xs idx = [ y | (x,y) <- xs, x == idx]

-- Remove uppercase
f04func st = [c | c <- st, c `elem` ['a'..'z']]

-- Set to lowercase
t05list = ['A'..'Z']
t06list = ['a'..'z']
t07list = zip t05list t06list
t08list = "AAAbbbCCCdddEEEfff"

t09list = zip [1..10] [11..20]

f05func c = (f03func t07list c) !! 0
f06func st = [ f05func c | c <- st, c `elem` ['A'..'Z']]
f07func st = [if x `elem` t05list then f05func x else x | x <- st]
f08func ls = [a+b | (a,b) <- ls]

f09func [] = error "Cant call head on an empty list, dummy!!"
f09func (x:_) = x

f10func all@(x:xs) = "First letter is: " ++ [x] ++ ", Intire word is: " ++ all

f11func r h = 
    let sidearea = 2 * pi * r * h 
        toparea = pi * r * 2 
    in sidearea + 2 * toparea

f12func a b =
    let a' = a*a
        b' = b*b
    in a' + b'

f13func xs = [bmi w h | (w, h) <- xs]
    where bmi weight height = weight / height * 2

-- Reverse
f14func [] = []
f14func (x:xs) = f14func xs ++ [x]

-- Eksempel:
-- pair' a b = (a, b)
-- f15func pair' t05list t06list
f15func f [] [] = []
f15func f (x:xs) (y:ys) = f x y : f15func f xs ys