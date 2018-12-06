**************
*Compilazione*
**************
Nella directory rpi-telemetry lanciare

    $ cmake -Bbuild -H.
    $ make -Cbuild
    $ cd build
    $ make install

Se tutto è andato per il verso giusto con il make genererai gli esegubili. 

************************
*Avvio modalitá normale*
************************
Ora basterà lanciare il publisher e subscriber con argomento il file config .lua che è nella directory principale. Dalla directory rpi-telemetry lanciare:

    $ build/sub cfg.lua

in alternativa all ultimo comando si puó usare: install/bin/sub cfg.lua

Da un altro terminale sempre da rpi-telemetri:

    $ build/pub cfg.lua

in alternativa all ultimo comando si puó usare: install/bin/pub cfg.lua

Per terminare ovviamente basta un Ctrl+C sul terminale del pub in questo modo il subscriber resta in ascolto

*********************
*Avvio modalitá test*
*********************
Se non siamo collegati ad un can che ci fornisce un cambiamento di stato (da running a idle o viceversa) non ha molto senso avviare in modalitá normale dato che la fsm fará sempre il publish e mai la flush cache
La modalitá di test permette di forzare i cambiamenti di stato all interno della fsm.
Il subscriber viene lanciato in maniera identica alla modalitá normale:

    $ build/sub cfg.lua

in alternativa all ultimo comando si puó usare: install/bin/sub cfg.lua

Da un altro terminale sempre da rpi-telemetri avviamo ora il publisher, a differenza di prima aggiungiamo un terzo argomento.
Questo argomento é una stringa di numeri interi, ogni numero identifica uno stato della fsm da riprodurre una volta lanciato il comando.
Terminati i comandi della all interno della string il programma si fermerá automaticamente con lo stato della fsm "stop".
I comandi e i rispettivi stati da forzare sono:
 0 -> publish
 1 -> store in cache (cache)
 2 -> restore from cache and publish (flush)

    $ build/pub cfg.lua stringaDiComandi

esempio: build/pub cfg.lua 01122 fará publish, cache, cache, flush, flush

in alternativa all ultimo comando si puó usare: install/bin/pub cfg.lua stringaDiComandi

Per terminare prima della fine dei comandi inseriti basta un Ctrl+C sul terminale del pub in questo modo il subscriber resta in ascolto

******
*note*
******

- in caso di messaggio "MQTT: Connection refused" lanciare il comando:
    $ mosquitto -d
