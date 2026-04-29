# 132_Stanescu_Matei_Tema_2_Poo_City_Builder

Joc city builder in consola.

Jucatorul are 4 resurse principale: bani, energie, populatie, fericire

Jucatorul poate folosi urmatoarele comenzi pentru a construi cladiri:

build house (Construieste o casa care ofera 3 populatie si consuma 3 energie.) - 50$

build generator (Construieste un generator care produce 9 energie, dar are nevoie de 2 angajati care sunt luati, cu prioritate, din populatia totala.) - 75$

build factory (Construieste o fabrica. Fabrica consuma 6 energie.) - 100$
    (Fabrica poate avea maxim 5 angajati si produce bani in functie de numarul acestora si de numarul de ture consecutive in care a fost activa.)
    (Fabrica produce poluare la finalul fiecarei ture, daca este activa. O fabrica poate fi dezactivata folosind comanda "deactivate (index)".)
    (O data ce a fost dezactivata, poluarea va scadea cu jumatate pe tura.)
    (Fabrica poate fi activata la loc folosind comanda "activate (index)")
    (La finalul fiecarei ture, fericirea scade cu poluare totala // 10.)

build park (Construieste un parc. Parcul ofera 10 fericire, dar consuma 20$ pe tura.) - 75$


pentru a trece la tura urmatoare se foloseste comanda "next".
pentru a opri jocul se foloseste comanda "exit".

La finalul fiecarei ture, daca energia produsa este mai mica decat energia necesara, toate casele si fabricile raman fara curent. Fabricile fara curent nu mai produc bani. Din casele fara curent vor pleca la inceputul fiecarei ture oameni, prima oara unul singur iar apoi doi. ( simbolul ! )

La finalul fiecarei ture, daca fericirea este negativa, vor pleca oameni din case, nr de oameni care pleaca este egal cu |happiness|.

Scopul jocului este ca jucatorul sa ajunga la 10000$ in 20 de ture.



Pentru a realiza proiectul am folosit concepte de programare orientata pe obiecte, inspirat de modelul Unity, care foloseste compunere bazata pe clase numite componente. Componentele sunt clase care mostenesc din clasa de baza Component.
Fiecare cladire are propria clasa mostenitoare din clasa de baza building si propriile componente. 
Metodele componentelor sunt apelate in interiorul functiilor virtuale suprascrise in clasele cladirilor. 
Functiile virtuale sunt OnTurnStart, OnTurnEnd, OnActivate, OnDeactivate.
Logica generala este realizata de clasa singleton GameManager.
Procesarea comenzilor si afisarea UI-ului are loc in main. 