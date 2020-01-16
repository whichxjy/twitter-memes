# Twitter memes

My solution to the challenge ["find Twitter memes with suffix arrays"](https://jvns.ca/blog/twitter-memes-challenge/).

## Build

```
git clone https://github.com/whichxjy/twitter-memes
cd twitter-memes
mkdir build
cd build
cmake ..
make
```

## Usage

1. Download [twitter.db.gz](https://www.dropbox.com/s/5xnaf1mbdr1424p/twitter.db.gz?dl=0) and extract `twitter.db` from it.

2. Run `./meme` in `build` directory:

    ```
    meme [OPTION...]

    -d, --db arg     Path to SQLite database with tweets
    -s, --size arg   The size of the common substring (default: 40)
    -n, --num arg    Minimum number of suffixes that contains the common substring (default: 5)
    -l, --limit arg  Limitation to the number of tweets to search from (default: 1000)
    ```

## Example

Run `meme -d /path/to/twitter.db`:

```
[Common substring:  a disclaimer and would like to mention 

==================================================[ Tweet ]==================================================
⚠ ATTENTION

due to twitter's new policy, i'm posting a disclaimer and would like to mention that i am not any of t… https://t.co/tzJRIIfjm1
=============================================================================================================

==================================================[ Tweet ]==================================================
ATTENTION

due to twitter's new policy, i'm posting a disclaimer and would like to mention that i am not any of the… https://t.co/e8wre8ftPy
=============================================================================================================

==================================================[ Tweet ]==================================================
due to twitter's new policy, i'm posting a disclaimer and would like to mention that i am not any of the celebritie… https://t.co/RBFygQNOMY
=============================================================================================================

==================================================[ Tweet ]==================================================
ATTENTION 

due to Twitter’s new policy, I’m posting a disclaimer and would like to mention that I am not any of th… https://t.co/QcepQojThz
=============================================================================================================

==================================================[ Tweet ]==================================================
ATTENTION 

due to Twitter’s new policy, I’m posting a disclaimer and would like to mention that I am not any of th… https://t.co/APBBMnGxeu
=============================================================================================================

==================================================[ Tweet ]==================================================
due to twitter's new policy, i'm posting a disclaimer and would like to mention that i am not any of the celebritie… https://t.co/6YkEhOwOjf
=============================================================================================================

==================================================[ Tweet ]==================================================
due to twitter's new policy, i'm posting a disclaimer and would like to mention that i am not any of the celebritie… https://t.co/rqDlvdmdDz
=============================================================================================================
```