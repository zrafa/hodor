ssh odor@172.16.0.10 "killall apriltag_demo"
ssh odor@172.16.0.10 apriltag_demo | stdbuf -o 0 grep distancia | ./april_sdl 
