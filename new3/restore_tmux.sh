#!/bin/sh

SN="newhope"
tmux has-session -t $SN &> /dev/null

if [ $? != 0 ]
then
  cd ~/projects/newhope/engine
  tmux new-session -s $SN -n engine -d 'vim -p world.lua person.lua'

  cd ~/projects/newhope/ui/test
  tmux new-window -t "$SN" -n ui 'vim -p ../interface/interface.h ../interface/interface.c'

  cd ~/projects/newhope/ui/test
  tmux new-window -t "$SN" -n shell

  tmux select-window -t $SN:1
fi

tmux attach -t $SN
  
# vim: ts=2:sw=2:sts=2:expandtab
