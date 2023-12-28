echo "Compilación y pruebas"
echo "=====================\n"

make -f makefile clean 2>&1
timeout 30s make -f makefile valgrind-ejemplo 2>&1

RETVAL=$?
if [ $RETVAL -eq 2 ]; then
    echo ""
    echo -e "\033[1;31m¡ERROR CORRIENDO VALGRIND! Hubo algún error en la entrega, las pruebas no pasan.\033[0m"
    echo ""
    exit 1
fi

if [ $RETVAL -eq 124 ]; then
    echo ""
    echo -e "\033[1;31m¡TIMEOUT! Hubo algún error en la entrega, las pruebas no pasan.\033[0m"
    echo ""
    exit 2;
fi

echo ""
echo "El programa llega al final con exito."
echo ""
echo ""
echo "TODAS LAS PRUEBAS SE EJECUTARON CON ÉXITO."
echo ""
echo ""

exit 0
