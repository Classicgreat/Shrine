#!/data/data/com.termux/files/usr/bin/bash

tmux new-session -d -s "$(basename "$dir")"
# tmux split-window -h
# tmux resize-pane -R 30
# tmux send-keys -t "$(basename "$dir"):0.1" "clear" Enter
tmux send-keys -t "$(basename "$dir"):0.0" "./start.sh" Enter
tmux attach-session -t "$(basename "$dir")"
