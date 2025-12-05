# New

This project is a fork of [AlphaZero Pybind11](https://github.com/bhansconnect/alphazero-pybind11).
After cloning the repository, run: 

```
git submodule update --init
```

### Heuristics

Since we're doing this with very limited compute, it's important to use heuristics to speed up learning. There are four ways we do this:

1) Reward shaping

   Implemented in `src/nichess_gs.cc` `scores` method.

   If the game is not over after X moves, we award the win to the player with more material (a weighted sum of all pieces). If both players have roughly equal material, the game is scored as a draw.

   X should be gradually increased from 50 to 200 and the heuristic should eventually be removed so that reward is only given for actually winning the game.

2) Heuristic value and policy during MCTS search

   Implemented in `src/play_manager.cc` `update_inferences` method.

   Instead of
   ```
   game.v = v.row(i);
   game.pi = pi.row(i);
   ```
   We have
   ```
   game.v = 0.1 * v.row(i) + 0.9 * heuristic_v
   game.pi = 0.1 * pi.row(i) + 0.9 * heuristic_pi
   ```

   As the AI becomes stronger, the heuristic weights should be decreased and the neural network weights should be increased. This achieves decent performance but it might be too slow. I'll play around with it a bit and see if it's worth using.

3) TODO: Alpha-beta search after MCTS

   This will be implemented in `src/play_manager.cc` `play` method.

   After running MCTS, we will run a shallow alpha–beta search and use it to form an additional policy estimate. Instead of:
   ```
   const auto pi = mcts.probs(temp);
   ```
   we'll do: 
   ```
   const auto mctsPi = mcts.probs(temp);
   const auto alphaBetaPi = game.gs->alphaBetaPi();
   const auto pi = 0.1 * mctsPi * pi + 0.9 * alphaBetaPi
   ```

4) TODO: Pretraining data

   Generate self-play data with alpha-beta search and heuristics.

For Nichess 1 we used (1) and (4). (2) and (3) are new ideas currently being tested.



# AlphaZero Pybind11 (Old README)

This is a modified implementation of AlphaZero. It takes some inspiration from [Learning From Scratch by Thinking Fast and Slow with Deep Learning and Tree Search](https://davidbarber.github.io/blog/2017/11/07/Learning-From-Scratch-by-Thinking-Fast-and-Slow-with-Deep-Learning-and-Tree-Search/) and some of my own modifications for caching positions. The actual core loops of games and MCTS are implemented in C++ for performance and memory reasons. Python is used for the Neural Network and data packaging.

## How to Install

### Prereqs

This project has a few prereqs, but should be usable on Linux, Windows, or Mac.

1. You need a C++ toolchain, ninja, and the meson build system.
All of this information can be found in [the meson installation guide](https://mesonbuild.com/SimpleStart.html).
Just follow along for whichever platform you use.

1. If using an nvidia gpu with cuda, you need the required video drivers.
This is a complex topic that is system specific. Google it for your OS.

1. You need a python3 setup. You can use the system built in python with pip or virtual environments.
I am not really a fan of that. I would advise [getting Anaconda](https://www.anaconda.com/).

### Python Env Setup

If using conda, you can follow this to get all of the python libaries installed:

1. Create and activate a new environment (on windows make sure to use the Anaconda prompt if not in path)
   ```
   conda create -n torch python=3.9
   conda activate torch
   ```

1. Install [pytorch](https://pytorch.org/get-started/locally/) as recomended for your system. Just select your OS, Conda, and the compute platform.

1. Other packages to install: `conda install tqdm matplotlib flask flask-cors`

1. Install aim, it is used for visualization of training. Sadly it is not yet supported by conda, so just: `pip install aim`. On Windows, this is problematic due to [this issue](https://github.com/aimhubio/aim/issues/2064). To make things still work, Windows, doesn't get any of the nice web logging. Maybe it is better to use WSL or dual boot linux.

### Build the Project

Building the project is pretty simple once you have the dependencies.

1. Setup the project with `meson setup build --buildtype release`. For M1 macs, you probably have to add `--cross-file m1-cross-file.txt` as well. This is due to python and its packages still generally being installed as x86_64.

1. Build the C++ `ninja -C build test`. There will be warnings, but my minimal tests should pass. For Windows, you may need to build through the meson wrapper with `meson compile -C build && meson test -C build`

## How to Use

### Just Running Something

To start training a network, just run: `python src/game_runner.py`

To track the progress, you can launch aim with: `aim up` from the project directy (make sure the torch env is activated).
Go the to Metrics tab and start adding metrics. [Guide on metrics explorer](https://www.youtube.com/watch?v=7LUT9judVTQ).

> Note: The first iteration will use a random player, and is likely to run much faster than following iterations.
If following iterations are too slow, try lowering the number of games played by changing `SELF_PLAY_BATCH_SIZE`.
You can also lower the tree search depth by changing `nn_selfplay_mcts_depth`.

After a couple of iterations have finished, you can end the program and have the networks directly compete in a tournament. For a faster tournament, run `python src/monrad.py`. For a full roundrobin, run `python src/roundrobin.py`

### Important Files for Users

Everything is a bit of a mess and config could definitely use more cleanup, but here are the important files:

 - `src/game_runner.py`: Main python entrypoint for training. Lot of config options.
 - `src/neural_net.py`: The neural network implementations and config.
 - `src/monrad.py`: Runs a monrad tournament between networks put in `data/bench`. Is be much faster than roundrobin.
 - `src/roundrobin.py`: Runs a roundrobin tournament between networks put in `data/bench`. Can take a long time to run, but is very accurate.
 - `src/play_agent.py`: Lets the user play a network. Requires move selection customization (look at the commented out block).
 - `src/play_analysis`: Compares the network to itself with increasingly more MCTS searches. Can be useful for debugging and telling how good a network is.
 - `src/game_state.h`: This is the interface that must be implemented to add a new game.
 - `src/*_gs.(h|cc)`: Implementations of various games.
 - `src/py_wrapper.cc`: The definition of the python api that gets exposed.
 - `src/meson.build`: The build definition that must be edited when adding a new game.

In general if you want to add something new, just start by coping from some other game and then editting. That will probably make life easier. Also, don't forget to rebuild before running the python app again.
