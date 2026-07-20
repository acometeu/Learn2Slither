PROJECT DESCRIPTION :
AI Model trainer for a classic snake game.
Must use the Q_learning method with either a Q_table or a neural network (I used a Q_table).
Must have the possibility to test the AI in a visual interface (game representation)
Constraint : the AI model can only see the board horizontaly and vertically from it's head



Q_LEARNING EXPLANATION:
the Q_leaning method (with Q_table) assign values to a table from actions taken at a given state, with this formula :
Q[s,a] = Q[s,a] + α(r + γ Q_max[s+1,a+1] - Q[s,a])

the Q_learning method with a Q_table rely on 3 parameters : 
-exploration rate (ε) : Rate to take a random action instead of current best Q[s,a] (1 : explore at each action, 0 : always choose direction with best Q_value).
-learning rate (α) : rate which new value impact current Q_value of Q_table (1 : replace completly old value, 0 : new value have no impact)
-future reward significance (gamma) : significance of next state value compare to direct reward (1 : same significance


COMPILE :
cmake -S . -B build
cmake --build build

LAUNCH :
'''./build/Learn2Slither -X xx --XXX xx'''
ex :
./build/Learn2Slither -vs 3 --session 100000 -b 10 --import saves/intermediate_classic.txt --no_print --learning_rate 0.005
./build/Learn2Slither -vs 3 --session 1000 -b 20 --import saves/20x20_complex.txt  --no_print --no_learn --strat complex
./build/Learn2Slither -vVs 3 --session 3 -b 10    --import saves/20x20_intermediate.txt --no_learn --step
./build/Learn2Slither -vs 3 --session 100000 -b 10 --export saves/test.txt --no_print



ARGUMENTS :
-v,--verbose : Flag to toggle verbose
-?,-h,--help : Display this paragraph in console
--session : Choose the number of game sessions to play/train (default : 1)
-s,--snake_size : Snake size at start of session (default : 3)
-b,--board_size : Board size of the game, in square shape (default : 10)
--no_learning,--no_learn : Flag to desactivate learning, great to test/compare efficacity of specifics trained models
--no_print : Flag to desactivate printing snake informations in console, great for fast training
-e,--epsilon,--exploration_rate : Agent exploration rate probability, min = 0, max = 1 (default : 0.005)
-a,--alpha,--learning_rate : Agent significance of new actions for training, min = 0, max = 1 (default : 0.1)
-g,--gamma,--future_reward_weight : Agent significance of future reward compare to instant reward for training, min = 0, max = 1 (default : 0.3)
-V,--view,--visual : Flag to display graphic interface of the snake game
--snake_speed,--speed : Game latency in miliseconds, for --visual mode only (default : 500)
--step : Flag to toggle step by step mode, for --visual mode only
--import,--import_path : Take a path to import q_table values
--export,--export_path : Take a path to export q_table values
--strat,--state_strat,--stat_strategy : choose the strategy to encode the Q_table with (default : intermediate)
  ."simple", "simple strategy" (fast learning, lowest space, worse results) [state only contain, red apple, green apple and obstacle informations from snake vision]
  ."intermediate", "intermediate strategy" (medium speed learning, medium space, best results) [simple strategy + snake head aproximate position]
  ."complex", "complex strategy" (slow learning, huge space, medium results, dependant of training board size used) [simple strategy + exact snake head position]




SOURCES:
https://www.youtube.com/watch?v=0iqz4tcKN58
