Pallanguzhi: A Digital Mancala Game

📜 Project Overview

This project is a digital implementation of Pallanguzhi (also known as Pallankuli, Alaguli Mane, or Vamana Guntalu), a traditional two-player mancala game originating from South India.

The game is developed in C using the raylib library, serving as a learning exercise in game logic implementation, graphic rendering, and state management within a simple game loop environment.

Features:

Classic 2-row, 14-pit game board layout.
Two-player mode (Human vs. AI).
Implementation of the core "Sowing" and "Capture" rules.
Real-time score tracking.

🛠️ Installation and Setup

To compile and run this game, you must have the following installed on your system:
C Compiler: GCC or Clang is recommended.
raylib Library: The raylib library must be installed and configured for your environment. (Refer to the official raylib documentation for detailed installation instructions).

🎮 How to Play

Game Board and Setup

The Pallanguzhi board is divided into two rows of seven pits each, totaling 14 pits.

Component Description

Your Side (Player)
The bottom row of seven pits.

AI Side (Opponent)
The top row of seven pits.

Initial Seeds

Each of the 14 pits starts with 5 seeds (or the configured number of seeds).

Objective

The player who captures the most seeds by the end of the game wins.

The Turn Sequence

Selection: When it is Your Turn, click on any pit on Your Side that contains seeds.

Sowing: All the seeds are taken from the selected pit and are distributed, one by one, into the successive pits in a counterclockwise direction around the board. This includes pits on both your side and the AI's side.

Continuation (Relay):

If the last seed you drop lands in a pit that already contains seeds, your turn continues. You immediately pick up all the seeds from next pit and continue sowing them in the counterclockwise direction.

This "relay" continues until the last seed lands in an empty pit.

The Capture Rule (The "Harvest")

Your turn ends only when the very last seed you are holding lands in a pit and next pit is empty.

Landing on Empty Pit: If your last seed lands in an empty pit and the pit next to it is empty.

- Then we declare it as barren pit and look at the next pit, if it has seeds you take all of them
- Then we declare it as barren pit and look at the next pit, if it has no seeds in it, you take no seeds.

Storing Captured Seeds: All captured seeds are added to your Score Pit (reflected in the Your Score counter).

Game End and Winner

The game ends when score difference between the players is higher than number of seeds in the board.

🖼️ Game Screenshot

A snapshot of the current game interface:
<img width="802" height="707" alt="image" src="https://github.com/user-attachments/assets/be5a53c7-356d-4b77-b9df-ab59be0dbdac" />


(Note: The game is configured for a Human vs. AI match, with player scores tracked at the top left.)

💡 Learning Objectives

This project is intended to reinforce the following concepts:

raylib Basics: Window creation, text rendering, drawing shapes (circles, rectangles).

Game State Management: Implementing a state machine to track whose turn it is, the scores, and the seed counts in each pit.

Game Logic: Implementing the core rules (sowing, movement, capture) using arrays or structs in C.

User Input: Handling mouse clicks to select pits.

AI Implementation (Basic): A simple AI algorithm (e.g., picking the pit that yields the largest immediate score or a random valid pit).
