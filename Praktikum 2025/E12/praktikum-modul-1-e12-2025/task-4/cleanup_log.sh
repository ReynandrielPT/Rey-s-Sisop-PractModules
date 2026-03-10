#!/bin/bash

user=$(whoami)
path="/home/$user/metrics/aggregated"


find "$path" -name "metrics_agg_*" -type f -mmin +720 -exec rm {} \;
