INSTRUCIÓNS COMPILACIÓN
1) Pode que sexa necesario que lle dea permisos de compilación ao script
que permite a compilación do código ("compilar.sh"). Para iso, sitúese
no directorio no que se atopa o script e execute:
>chmod +x compilar.sh

2) Unha vez outorgados os permisos necesarios, proceda a executar o script:
>./compilar.sh

3) Este fai uso dun makefile, creando todos os .o correspondentes, así como
o executable "programa"

4) Asegúrese de dispoñer do ficheiro "concurrentSum.go" no directorio actual 
e execute:
>./programa concurrentSum.go
ou
>./programa
se quere comprobar a xestión do erro de non enviar o ficheiro como parámetro.
