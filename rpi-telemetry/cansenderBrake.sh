#!/bin/bash

while true; do
	cansend can0 0B0#021E
	sleep .1
	cansend can0 0B0#0228
	sleep .1
	cansend can0 0B0#0232
	sleep .1
done


