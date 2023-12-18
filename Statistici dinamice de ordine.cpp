/*
*  Ciobanu Radu-Rares
*  Grupa 30222
	*In acest cod am testat algoritmii pentru statistici dinamice de ordine, cat si complexitatea operatiilor de management, si anume:
		-BuildTree
		-Order Statistic Selection
		-Order Statistic Delete

	 -pentru functia demo am creat cu ajutorul functiei buildTree arborele binar de cautare perfect echilibrat si am testat corectitudinea 
	  functiior selectieOS si deleteOS si s-au dovedit a functiona drept asteptarilor
	 -pentru afisarea arborelui am folosit funtia de pretty print a uni arbore binar de cautare folosita si in laboratorul anterior 
	 -pentru generarea graficelor am numarat operatiile de pe fiecare functie pentru un numar sugerat de teste, iar complexitatea acestora ne-a 
	  reziltat in O(logN) unde N este numarul de noduri ale arborelui

*/

#include <stdio.h>	
#include <iostream>
#include <random>
#include "Profiler.h"
using namespace std;

Profiler p("OrderStatistics");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTE 5

typedef struct node {

	int size;
	int key;
	struct node* parent;
	struct node* left;
	struct node* right;

}NodeT;

NodeT* create(int key) {

	NodeT* q = new NodeT;
	q->key = key;
	q->size = 1; 
	q->left = NULL;
	q->right = NULL;
	q->parent = NULL;

	return q;
}

NodeT* buildTree(NodeT* x, int left, int right, int q) {

	Operation attr = p.createOperation("Atribuiri", q);
	Operation comp = p.createOperation("Comparatii", q);

	comp.count();
	if (left > right) {
		return NULL;
		}
	int mij = (right + left) / 2;
	attr.count();
	NodeT* pr = create(mij);
	attr.count();
	pr->parent = x;
	attr.count();
	pr->left = buildTree(pr, left, mij-1, q);
	attr.count();
	pr->right = buildTree(pr, mij+1, right, q);
	comp.count();
	if (pr->right) {
		attr.count();
		pr->size += pr->right->size;
	}
	comp.count();
	 if (pr->left) {
		 attr.count();
		pr->size += pr->left->size;
	}
	return pr;

}

NodeT* minim(NodeT* x) {

	while (x->left != NULL) {
		x = x->left;
	}
	return x;
}


NodeT* selectieOS(NodeT* x , int index,int q) {

	Operation attr = p.createOperation("Atribuiri", q);
	Operation comp = p.createOperation("Comparatii", q);
		
	int r = 0;
	comp.count();
	if (x->left == NULL) {
		r = 1;
	}
	else
		r = x->left->size + 1;
	if (r == index) {
		return x;
	}
	else if (index < r) {
		return selectieOS(x->left, index, q);
	}
	else 
		return selectieOS(x->right, index - r, q);

}

NodeT* deleteOS(NodeT* x, NodeT* l,int q) {

	Operation attr = p.createOperation("Atribuiri", q);
	Operation comp = p.createOperation("Comparatii", q);

	NodeT* t = new NodeT;
	comp.count();
	if (x == NULL) {
		return NULL;
	}
	comp.count();
	if (l->key < x->key) {
		attr.count();
		x->left = deleteOS(x->left, l, q);
	}
	else if (l->key > x->key) {
		attr.count();
		x->right = deleteOS(x->right, l, q);
	}
	else {
		comp.count();
		if (x->left == NULL) {
			attr.count();
			t = x->right;
			free(x);
			return t;
		}
		else if (x->right == NULL) {
			attr.count();
			t = x->left;
			free(x);
			return t;
		}
		attr.count();
		t = minim(x->right);
		attr.count();
		x->key = t->key;
		attr.count();
		x->right = deleteOS(x->right, t, q);
	}
	x->size = 1 + (x->left ? x->left->size : 0) + (x->right ? x->right->size : 0);
	return x;
}

//NodeT* succesor(NodeT* x) {
//	NodeT* l = new NodeT;
//	if (x->right == NULL) {
//		return minim(x->right);
//	}
//	l = x->parent;
//	while ((l != NULL)&&(x == l->right)) {
//		x = l;
//		l = l->parent;
//	}
//	return l;
//}

void prettyBinar(NodeT* rad, int count) {

	if (rad != NULL) {
		for (int i = 0; i < count; i++) {
			printf("   ");
		}
		printf("%d size %d\n", rad->key, rad->size);
		prettyBinar(rad->left, count + 1);
		prettyBinar(rad->right, count + 1);
	}
}

void demo() {

	int n = 11;
	NodeT* rada = NULL;
	rada = buildTree(rada, 1, n, 0);
	prettyBinar(rada, 0);
	NodeT* s1 = selectieOS(rada, 4, 0);
	NodeT* s2 = selectieOS(rada, 2, 0);
	NodeT* s3 = selectieOS(rada, 9, 0);
	deleteOS(rada, s1, 0);
	printf("\n");
	prettyBinar(rada, 0);
	deleteOS(rada, s2, 0);
	printf("\n");
	prettyBinar(rada, 0);
	deleteOS(rada, s3, 0);
	printf("\n");
	prettyBinar(rada, 0);
}

void grafice() {

	for (int n = 100; n <= MAX_SIZE; n += STEP_SIZE) {
		for (int teste = 0; teste < NR_TESTE; teste++) {
			int k = n;
			NodeT* arb = new NodeT;
			arb = NULL;
			arb = buildTree(arb, 1, n, n);
			for (int i = 0; i < n; i++) {
				NodeT* s = new NodeT;
				s = NULL;
				s = selectieOS(arb, rand() % k + 1, n);
				deleteOS(arb, s, n);
				k--;
			}
		}
	}
	p.addSeries("Complexitate", "Atribuiri", "Comparatii");
	p.showReport();

}

int main() {

	demo();
	//grafice();
	return 0;
}