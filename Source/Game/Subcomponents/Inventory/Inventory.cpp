#include "Inventory.h"

#include "../../Items/Empty.h"

Inventory::Inventory() {
  for (int i = 0; i < 10 + 60; i++)
    items.emplace_back(new Empty());

  GiveItem<Pickaxe>(ToolType::BronzePickaxe, 1);
  GiveItem<Shovel>(ToolType::BronzeShovel, 1);
  GiveItem<Axe>(ToolType::BronzeAxe, 1);

  GiveItem<Block>(BlockType::Wood, 32);
  GiveItem<Wall>(WallType::Wood, 32);
}

void Inventory::Swap(int a, int b) {
  Ref<Item> copy = items[b];
  items[b] = items[a];
  items[a] = copy;
}