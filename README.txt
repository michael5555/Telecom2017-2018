Plaats de files in Elements folder in elements/local.
Plaats de files in scripts/library in de scripts/library folder van de click installatie.

Om dit project te laten werken kan er gebruik gemaakt worden van handlers.
Voor de Mobile IP implementatie zijn er 3 verschillende acties: adverteren, registreren en pakketten tunnelen.
Adverteren wordt gedaan met gebruik van een handler. (de)Registeren en tunnelen gebeuren dan automatisch.

De mobile node begint in een home situatie via het scriptje to_foreign.sh kan de mobile node 'weg' gaan.
Het scriptje to_home.sh zorgt ervoor dat de mobile node terugkeert.

Als de mobile node terugkeert van weg geweest te zijn, moet er ook geadverteerd worden alvorens er terug receptie is van de ICMP Echoes.

Adverteren,registreren en tunnelen gebeuren nu automatisch,maar kunnen ook via een handler uitgevoerd worden.

Adverteren via agent:

    home agent:
                telnet localhost 10002
                write home_agent/advertisementsource.sendAdvertisement
    foreign agent:
                telnet localhost 10003
                write foreign_agent/advertisementsource.sendAdvertisement

Adverteren via sollicitation van mobile node:
                telnet localhost 10001
                write mobile_node/sollicitationsource.sendSollicitation



De voornaamste situatie

    1.Mobile Node thuis, icmp echoes worden ontvangen.
    2. sudo ./to_foreign.sh (mobile_node gaat 'weg')
    4. automatisch geadverteerd via foreign agent advertisement of sollicitation
    5. ICMP Echoes worden ontvangen
    6. sudo ./to_home.sh (mobile_node keert terug)
    8. automatisch geadverteerd via home agent advertisement of sollicitation
    9. pakketten worden terug ontvangen.


Er is ook een handler voorzien om de ICMP Router lifetime in te stellen.




