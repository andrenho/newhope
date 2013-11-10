#!/bin/sh

SN="newhope"
tmux has-session -t $SN &> /dev/null

if [ $? != 0 ]
then
  cd ~/projects/newhope/engine
  tmux new-session -s $SN -n engine -d

  cd ~/projects/newhope/ui/test
  tmux new-window -t "$SN" -n ui

  cd ~/projects/newhope/ui/test
  tmux new-window -t "$SN" -n shell

  cd ~/projects/newhope_old/new2
  tmux new-window -t "$SN" -n old

  tmux select-window -t $SN:1
fi

tmux attach -t $SN
  
# vim: ts=2:sw=2:sts=2:expandtab
