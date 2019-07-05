#!/bin/bash

while true; do
	cansend can0 0B0#0100
	sleep .1
	cansend can0 0B0#010A
	sleep .1
	cansend can0 0B0#0113
	sleep .1
done