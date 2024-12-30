The game involves a number of dynamically generated box actors that can be damaged and destroyed during gameplay.
When box's health drops to zero or below, box is destroyed and player earns a score.

Logic:

For the Health Reduction : The ADynamicBox class has a TakeDamage function which reduces the box's health when
damage is taken. If it reaches 0 or below , the box is destroyed.

For the Score Update: The Player's Score is updated by Calling AddScore in the player's character class.
This adds a prefixed amount based on the Json value of score parsed.

Challenges Faced:

The Json Parsing feature took longer than expected to build since there were few resources online for
learning about the required libraries.

How to test feature:

When the game runs , the static boxes will spawn at their provided locations.
The player can test the score system and the hp system by shooting the boxes with the 
basic projectiles and see if the score updates uniquely based on the box destroyed.
