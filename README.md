# Produkto_Gamyba
Program for checking whether a pharmaceutical product can be made, that is if required materials are available.
Product list is in an Excel file which also contains required materials.

Compiled with Qt 5.10.0 MinGW 32bit

# Description

Required files:

-Excel which contains products and components

-Eip file from "Rivilė" which has the latest warehouse information about component remainders


What the program does:
basically it checks wheather a product (from excel file) can be manufactured with given materials (eip file data).
Colors indicate that there are sub components.
If two or more products are used in the search, starting from the second product material remainings are counted as if the first product already consumed its needs. (So if we have 100, first product needs 20 and second needs 90 we can only manufacture the first one because after that we only have 80 which is not enough to make our second product).


Images:

![#1](screens/pradzia.png)

![#2](screens/produkto_paieska.png)

![#3](screens/produkto_paieska_2.png)

