/* carrega o valor 20 para o registrador V0
e depois soma 30. Na sequência chama a sub rotina
que está implementada a partir do endereço
0x0400. Por fim soma mais 20 ao valor do registrador V0 */
0x0200: 
    LD V0, 70 
    ADD V0, 30
    CALL 0x400
    ADD V0, 20
// carrega o valor 10 para V0 (limpando ele), e depois acrescenta 15
0x0400:
    LD V0, 10
    ADD V0, 15
    RET