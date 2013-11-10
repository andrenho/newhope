#!/bin/sh

SESSIONNAME="newhope"
tmux has-session -t $SESSIONNAME &> /dev/null

if [ $? != 0 ]
then
  tmux new-session -s $SESSIONNAME -n engine -d
  tmux send-keys -t engine "cd ~/projects/newhope/engine" C-m

  tmux new-window -n ui -c "~/projects/newhope/temp/ui"
fi

tmux attach -t $SESSIONNAME
  
# vim: ts=2:sw=2:sts=2:expandtab
