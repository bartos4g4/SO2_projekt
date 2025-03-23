# Problem jedzących filozofów
##	Opis problemu
Problem jedzących filozofów jest klasycznym przykładem problemu synchronizacji w systemach wielowątkowych. Grupa filozofów siedzi przy stole z talerzami przed każdym z nich, pomiędzy talerzami znajdują się widelce. Aby jeść filozof musi podnieść dwa widelce, lewy i prawy. Celem jest uniknięcie sytuacji gdy filozofowie głodują z powodu braku wolnych widelców.
##	Kompilacja oraz uruchomienie programu
- Aby skompilować program należy użyć polecania: g++ -o filozofowie projekt1.cpp -lpthread -pthread
- Po kompilacji w celu uruchomienia programu należy użyć polecenia: ./filozofowie
- Program uruchamiany był na systemie Ubuntu 24.04.1 LTS oraz przy użyciu powłoki Bash.
##	Wątki
Każdy filozof reprezentowany jest przez osobny wątek. Są one tworzone w funkcji main() oraz uruchamianie równolegle. Ich liczba jest równa ilości filozofów, podanych przez użytkownika. Każdy wątek wykonuje funkcję philosopher(), która symuluje cykl myślenia i jedzenia. Cykle te wykonują się określoną liczbę razy, podaną przez użytkownika.
##	Sekcje krytyczne
- Sekcje krytyczne występują w momencie gdy filozofowie próbują uzyskać dostęp do widelców.
- Synchronizacja została rozwiązana za pomocą mechanizmu spinlock. Jest on oparty na aktywnym oczekiwaniu. Funkcja lock() blokuje dostęp do widelca. Jeżeli jest on niedostępny, to wątek czeka aby umożliwić innym wątkom wykonanie operacji. Działa ona w pętli aż widelec stanie się dostępny. Funkcja unlock() zwalnia blokadę widelca.
- W celu uniknięcia zakleszczenia zaimplementowano rozwiązanie asymetryczne. Filozofowie o parzystym numerze starają się najpierw uzyskać dostęp do lewego widelca. Filozofowie o nieparzystym numerze do prawego widelca.



