import Fact

import System.Environment

main = do
    args <- getArgs
    if null args then putStrLn "Arguments expected"
    else putStrLn ("Input: " ++ head args)
    let nb = head args
    print (fac (toInt nb) )

listInt = [1..5]
listString = ["1", "2", "3", "4", "5"]
listzip = zip listString listInt

getElem xs idx = [ y | (x,y) <- xs, x == idx]
listSearch c ls = (getElem ls c) !! 0

toInt "" = 0
toInt s = listSearch s listzip

