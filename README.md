# Produkto_Gamyba
Program for checking whether a pharmaceutical product can be made, that is if required materials are available.

Compiled with Qt 5.10.0 MinGW 32bit

# Description

Required files(not present in the repository):

-Excel which contains products and components

-Eip file which has the latest warehouse information about component remainders


What the program does:
basically it checks whether a product (from excel file) can be manufactured with given materials (eip file data).
Colors indicate that there are sub components.
If two or more products are used in the search, starting from the second product material remainings are counted as if the first product already consumed its needs. (So if we have 100, first product needs 20 and second needs 90 we can only manufacture the first one because after that we only have 80 which is not enough to make our second product).


Images:

![#1](screens/pradzia.png)

