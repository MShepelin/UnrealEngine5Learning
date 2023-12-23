# Unreal Engine 5 Course

That's my game on Unreal Engine 5.

### Setup

I used First Person Shooter template to create my project.

### Version Control

This project uses Git VCS without LFS.

<img src="./diffExample.png" alt="diffExample" style="zoom:33%;" />

### Basic Classes

I changed all boxes on my level to instance a class with HP. I also added ammo pickup cubes, bullet count for the weapon.

<img src="./gameplayScreenshot1.png" alt="gameplayScreenshot1" style="zoom: 33%;" />

### Unreal Macros

I moved HP logic to a custom component written in C++ (HealthComponent) and used unreal macros to make HP editable in the range from 0 to 100.

```c++
UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", Meta = (ClampMin = "0", ClampMax = "100"))
int32 HealthPoints;
```

![healthPointsUProperty](./healthPointsUProperty.png)

### Basic UI

My game has basic UI and UI for pause menu. Pause mechanic also works (you can pause the game using "E" button).

<img src="./gameplayScreenshot2.png" alt="gameplayScreenshot2" style="zoom:33%;" />

###Pure Functions

As one article elegantly states "Pure nodes are executed on demand whenever their output is required by an impure node for it to execute". I added 2 pure functions (one with const C++ modifier, another - with BlueprintPure UE modifier). Also I created a logic that can work only with pure functions:

<img src="./pureFunction2.png" alt="pureFunction2" style="zoom:50%;" />

The GetHealthPointsExpensive function is called both before and after the value of HP is decreased. If the output was cached, this logic wouldn't work. However, because we call this function every time we need an output, it is executed twice and gives correct results.
<img src="./pureFunction1.png" alt="pureFunction2" style="zoom:50%;" />
