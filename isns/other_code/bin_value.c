#include <stdio.h>
#include <stdlib.h>
#include <ldap.h>

int main()
{
       LDAP            *ld;
       LDAPMessage     *res, *e;
       int             i;
       char            *a, *dn;
       void            *ptr;
       char            **vals;

       /* open a connection */
       if (ldap_initialize(&ld, "ldap://127.0.0.1:38989"))
            exit( 1 );

        int version = LDAP_VERSION3; 
        ldap_set_option( ld, LDAP_OPT_PROTOCOL_VERSION, &version );
       /* authenticate as nobody */
       if ( ldap_simple_bind_s( ld, "cn=root,dc=abc,dc=com", "123456" ) != LDAP_SUCCESS ) {
               ldap_perror( ld, "ldap_simple_bind_s" );
               ldap_unbind(ld);
               exit( 1 );
       }

       printf("--------------Hex value Modify-------------\r\n");
       LDAPMod l_mod;
       LDAPMod *apstMod[] = {&l_mod, NULL};
       char *att_val[] = {"%xBBDDFFF6543210000", NULL};
       l_mod.mod_op = LDAP_MOD_REPLACE;
       l_mod.mod_type = "isnsValue";
       l_mod.mod_values = att_val;
       
       if(ldap_modify_ext_s(ld, "isnsKey=%xABCDEF,ou=DD,dc=abc,dc=com", apstMod, 0, 0))
       {
           ldap_perror( ld, "ldap_modify_ext_s" );
           ldap_unbind(ld);
           exit(1);
       }
       /*
       printf("\r\nsleeping for 1 second...\r\n");
       sleep(1);
       printf("--------------Test Add Entry-------------\r\n");
       LDAPMod l_add[3];    
       LDAPMod *apstAdd[] = {&l_add[0], &l_add[1], &l_add[2], NULL};

       memset(l_add, 0, sizeof(l_add));     
       
       l_add[0].mod_type = "cn";
       char *att_val0[] = {"Kitty0", NULL};
       l_add[0].mod_values = att_val0;
       l_add[0].mod_op = LDAP_MOD_REPLACE;
       
       l_add[1].mod_type = "sn";
       char *att_val1[] = {"abcdefg Kitty", NULL};
       l_add[1].mod_values = att_val1;
       l_add[1].mod_op = LDAP_MOD_REPLACE;       

       l_add[2].mod_type = "objectClass";
       char *att_val2[] = {"person", NULL};
       l_add[2].mod_values = att_val2;
       l_add[2].mod_op = LDAP_MOD_REPLACE;
       
       if(ldap_add_s(ld, "cn=Kitty0,dc=abc,dc=com", apstAdd, 0, 0))
       //if(ldap_modify_ext_s(ld, "cn=Kitty0,dc=abc,dc=com", apstAdd, 0, 0))
       {
           ldap_perror( ld, "ldap_add_s" );
           ldap_unbind(ld);
           exit(1);
       }
       */
       /* close and free connection resources */
       ldap_unbind( ld );
       
       return 0;
}