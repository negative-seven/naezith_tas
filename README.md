# naezith_tas
TAS tool for Remnants of Naezith

## Usage
Run the executable after launching the game, then complete a level and press "Watch my last run". After restarting the replay, your movie file should play back successfully.

## Arguments
```
  -n [--no-alloc]
Use code injection method that does not utilise external memory allocation. Not guaranteed to be consistent - only set this if the software does not work otherwise.

  -f [--replay-file] <arg>
Filepath for replay file. Default: "inputs.txt".

  -s [--replay-size] <arg>
Maximum size of replay, in bytes. Can be decimal, hexadecimal (prefix 0x) or octal (prefix 0). Default: 0x1000.
```

## Replay file format

Replay files consist of lines corresponding to actions. A line takes on the format of:
```
 - <ticks to wait>
 - <space>
 - <number of actions>
 - for each action:
   - <space>
   - <'P' or 'R' character for pressed/released>
   - <space>
   - <input ID (from 0 to 4)>
 - <line feed>
```
