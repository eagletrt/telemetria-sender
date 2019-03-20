********************************
*Setup Raspberry PI per Muretto*
********************************

In questa pagina è raggruppato tutto quello che bisogna sapere sul Raspberry
e MQTT con parti di codice il più possibile 'Ctrl+C' 'Ctrl+V'
Di default il Raspberry ha come nome utente,hostname e password rispettivamente raspberry, pi, raspberry.

Username:pi
Password:telemetrypi
Hostname:telemetrypi
Wifi:eagleTRT
Password:eaglepiTRT

******************************
*Cambiare Hostname e Password*
******************************

Si può fare con sudo raspi-config per ora modificheremo la password
dopo aver fatto il primo login invece hostname si può benissimo fare così

sudo vim /etc/hostname
Scrivere 'telemetrypi'

sudo passwd
Scrivere 'telemetrypi' e riscriverlo ancora per confermare

***************************************
*Connettersi in automatico a reti WIFI*
***************************************

in /etc/wpa_supplicant/wpa_supplicant.conf

network={
        ssid="eagleTRT"
        psk="eaglepiTRT"
        key_mgmt=WPA-PSK
}

****************
*Connesione SSH*
****************

in boot creare un file "ssh" senza estensione
oppure se si ha la possibilità usare raspi-config

************
*IP Statico*
************

sudo vim /etc/network/interfaces

iface lo inet loopback

allow-hotplug wlan0
iface wlan0 inet manual
wpa-roam /etc/wpa_supplicant/wpa_supplicant.conf

auto wlan0
iface wlan0 inet static
address 192.168.1.200
gateway 192.168.1.1
netmask 255.255.255.0


auto eth0
iface eth0 inet static
address 192.168.1.200
gateway 192.168.1.1
netmask 255.255.255.0

********************
*SSH senza Password*
********************

ls ~/.ssh
cd ~/.ssh
rm id*
ssh-keygen //questo dovrà essere il path /home/pi/.ssh/id_rsa

ssh-copy-id pi@telemetrypi
oppure
cat ~/.ssh/id_rsa.pub | ssh pi@raspberrypi 'mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys'


*********************
*Inviare file in SSH*
*********************

scp myfile.txt pi@192.168.1.3:project
scp -r directory pi@192.168.1.3:project

****************
*Installare VIM*
****************

https://github.com/amix/vimrc

git clone --depth=1 https://github.com/amix/vimrc.git ~/.vim_runtime
sh ~/.vim_runtime/install_awesome_vimrc.sh

in ~/.vim_runtime/my_configs.vim

set number
set relativenumber
"% match brackets
set showmatch

colorscheme elflord

"remap for ijkl and replace insert mode with h
map i <Up>
map j <Left>
map k <Down>
noremap h i

"new empty line under the current line
nmap oo o<Esc>j












