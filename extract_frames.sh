#!/usr/bin/env sh
# Usage: ./extract_frames.sh input_video fps_to_extract resize_scale


out_dir="$1_analysis/$3-scale_$2-fps"

if [ ! -d "$out_dir" ]; then
    mkdir -p $out_dir
fi

ffmpeg -i $1 -r $2 -vf scale=iw*$3:-1 -f image2 $out_dir/%05d.png
