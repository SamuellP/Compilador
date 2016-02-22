#ifndef _HELLO_JNI_CPP_IMPL_H
#define _HELLO_JNI_CPP_IMPL_H

#include <boost/unordered_map.hpp>
//#include <boost/unordered_map.hpp>
#include <string>
#include <iostream>
#include <vector>

#ifdef __cplusplus
	extern "C" {

		#endif 

		void imprimirString(char str[]);
		int usaHash();
		int containsKey(char *key, char* scope);
		void putVariable(char *variable, char *type, char *scope);
		void adicionaParametro(char* id, char* parametro);
		void adicionaId(char* identificadorFuncao, char* id);
		char* getType(char* variable, char* scope);
		void putFunction(char *variable, char *type);
		int isFunction(char* id);

		#ifdef __cplusplus

	}

#endif

#endif