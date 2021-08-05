# ARM-LabyrinthGame
Group Project of a Labyrinth game made on an ARM board LandTiger LPC1768. Final score: 30L.

# Application Note: ExtraPoints_2 – Marco Smorti S281554
Laboratorio svolto in collaborazione con:
* Gabriele Scaffidi Militone - S288200
* Mattia Riola - S280169

Il progetto è strutturato in modo tale da utilizzare due timer: _Timer0_ impostato a _500μs_; RIT impostato a _25 ms_. Il timer0 si occupa di rilevare eventuali tocchi sullo schermo, mentre il RIT effettua polling per sapere se il joystick è in utilizzo.

# Libreria labyrinth
La libreria **labyrinth** è il core del progetto: essa definisce una serie di costanti che indicano il pixel di inizio della griglia di gioco, dei bottoni e del testo, oltre che delle costanti per l’impostazione del colore di ogni oggetto sopra citato. In coda alle costanti sono presenti tutta una serie di variabili che indicano la posizione del robot nel labirinto in forma di coordinate (x,y). Infine, sono presenti le funzioni che inizializzano il labirinto, cioè che costruiscono a schermo la struttura (vedere `init_labyrinth()`,`draw_panel()`,`draw_title()`,`draw_grid()`,`draw_buttons()`). 
* Una funzione molto utile è la `coord_transform(int x,int y)` che, date le coordinate x e y intere del robot, fornisce in uscita le coordinate in pixel del punto in alto a sinistra della casella in cui il robot si trova. Questa funzione viene richiamata ogni qualvolta è necessario modificare il contenuto di qualche cella (per disegnare un ostacolo, per cancellare la posizione vecchia del robot e per disegnare la nuova posizione). 
* Ulteriori funzioni della libreria labyrinth sono quelle che disegnano il robot: esso è disegnato sotto forma di triangolo, la cui funzione che disegna il triangolo a schermo risiede nella libreria GLCD ed è `LCD_DrawTriangle()`. Questa funzione, date le coordinate del pixel iniziale, una altezza, un colore ed una direzione (N,S,E,W) disegna il corrispondente triangolo orientato verso la destinazione data.
* La funzione `move_robot()` è modificata rispetto all’ExtraPoints1 poiché, oltre ad aggiornare la posizione del robot e a verificare la vittoria, ridisegna il robot nella posizione corretta a schermo tramite la `clear_robot()` prima e la `draw_robot()` dopo. Richiama inoltre la funzione `obstacle_scan()`.
* `Obstacle_scan()` è una funzione modificata dall’ExtraPoints1 che verifica,  data una direzione, se sono presenti ostacoli fino ad un range massimo definito con la costante `RANGE_SENSOR`. Se l’ostacolo viene rilevato, esso viene anche disegnato tramite la `draw_obstacle()`.
* La funzione `robot_explore()` non muove il robot, ma aggiorna la direzione (se essa viene modificata dal joystick) e richiama la `obstacle_scan()` per la ricerca di ostacoli.

# Libreria RIT & Joystick
La libreria joystick è stata modificata aggiungendo l’abilitazione dei PIN anche per i 4 tasti direzionali (oltre che per SELECT già presente) all’interno della funzione `joystick_init()`. La gestione di ciò che succede azionando il joystick è affidata alla funzione `RIT_IRQHandler()` presente all’interno della libreria RIT. La funzione utilizza una variabile victory che, se settata a 1 in caso di vittoria tramite la funzione `checkVictory()` della libreria labyrinth, disabilita l’utilizzo del joystick. La funzione è struttura tramite 5 IF che controllano se il pulsante SELECT o uno dei 4 tasti direzionali è stato premuto. Il primo caso è quello del pulsante SELECT: se esso viene schiacciato ne modifica la modalità di gioco agendo sulla variabile globale mode ed aggiorna il colore del robot. I successivi 4 IF azionano la `robot_explore()` ed eventualmente la `move_robot()` se si è in modalità MOVE soltanto dopo 1s di ritardo, agendo sulle variabili contatori (si usa 40 poiché il RIT è a 25ms). All’interno di ogni IF sono presenti più condizioni sui pin del joystick per evitare effetti indesiderati nel caso in cui l’utente spinga il joystick verso direzioni intermedie (es. NORDEST).  
