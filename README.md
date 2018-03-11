# Cuisine

Ce repo contient les ressources créés pour modifier la cuisine en bois de mon fils : https://twitter.com/hiteule/status/970345943072690176

* cuisine.sch : Schematic Eagle de la carte électronique
* cuisine.brd : Board Eagle de la carte électronique
* cuisine.ino : Code à faire tourner sur un Arduino Nano ou tout autre carte à microcontrôleur AVR

La carte a été pensée pour être alimenté en 12v afin de contrôler deux bandes de LEDs fonctionnant à la même tension.
Attention, l'emplacement des mosfet n'est pas prévu pour accueillir des radiateurs, on ne peux donc pas tirer un courant trop important.

Deux boutons poussoirs à LED doivent être connectés pour servir de commande. Il n'y a pas de résistance de tirage car celles-ci sont intégrées à l'Arduino Nano.

L'afficheur de type 7 segments est drivé par un TM1637.