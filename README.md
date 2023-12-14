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
