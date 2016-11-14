Git

master: Hauptprojekt
branch: Jeder arbeitet an einer Kopie des Hauptprojekts
commits: Sind Änderungen innerhalb einer „branch“
pull request: Anfrage zur Zusammenschließung von „branch“ und „master“
merge: fügt „branch“ und „master“ zusammen


Installation
Download und Installation für Mac/Windows https://desktop.github.com/
oder Linux https://git-scm.com/download/linux 

In Verzeichnis wechseln, in welches ihr die Sourcefiles wollt
Einmaliges kopieren des masters in das Verzeichnis mit: (wird ein eigener Ordner erstellt)
git clone https://github.com/koellemichael/sysprak.git


git status
- Zeigt unterschiede zwischen branch und master

git add <file>
- Fügt die file zu den Commits hinzu

git commit -m „message“
- Führt die Änderung durch im branch

git push
- Führt die Änderung durch im master

git pull
- Synchronisiert mit master

falls es zu Konflikten kommt, Konflikte manuell lösen und mit 
git commit -m „message“
git push
zum master hinzufügen
