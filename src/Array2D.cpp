#ifndef ARRAY2D_CPP
#define ARRAY2D_CPP

#include <stdio.h>
#include <string.h>

template <class T>
class Array2D
{
	T *** m_array;
	int m_nbLi;
	int m_nbCo;

public:
	//template <class T>
	Array2D(int _nbCo, int _nbLi)
	{
		allocate(_nbCo, _nbLi);
	}

	inline int width()
	{
		return m_nbCo;
	}

	inline int height()
	{
		return m_nbLi;
	}

	T*** getArray() {
	    return m_array;
	}

	inline T* GetElemAt(int _co, int _li)
	{
		if(_li >= m_nbLi || _co >= m_nbCo ||
           _li < 0       || _co <  0 ) return NULL;
		return m_array[_co][_li];
	}

	inline bool SetElemAt(int _co, int _li, T* _elem)
	{
		if(_li >= m_nbLi || _co >= m_nbCo ||
           _li < 0       || _co <  0 ) return false;
		T* elemToDestroy = GetElemAt(_co, _li);
		if(elemToDestroy != NULL) delete elemToDestroy;
		m_array[_co][_li] = _elem;
		return true;
	}

	void Resize(int _nbCo, int _nbLi)
	{
		if(_nbLi == m_nbLi && _nbCo == m_nbCo) return;

		// se souvenir du tableau contenant tous les T * et de la taille
		T ** save = m_array[0];
		int oldLi = m_nbLi;
		int oldCo = m_nbCo;

		// détruire le tableau d'indexage (on utilise pas deallocate parce que on ne veut pas libérer le tableau de contenu qu'on a sauvegardé juste au dessus, dans "save")
		delete [] m_array;

		// Reconstruire le tableau d'indexage a la bonne taille, ainsi que le tableau de contenu
		allocate(_nbCo, _nbLi);

		// Recopier ce qu'il est possible de recopier, et détruire ce qui 'dépasse' (dans le cas où la nouvelle taille est plus petite que l'ancienne)
		for(int li = 0 ; li < oldLi ; li++)
		{
			for(int co = 0 ; co < oldCo ; co++)
			{
				if(li < m_nbLi && co < m_nbCo)
				{
					m_array[co][li] = save[co][li]; // "li * oldCo + co"  calcule la position de l'objet dans le tableau de contenu 1D qui contient en fait toutes les lignes/colonnes du tableau 2D
				}
				else
				{
					delete save[co][li];
				}
			}
		}

		// Déstruction de l'ancien tableau de contenu
		delete [] save;
	}

protected:

	void allocate(int _nbCo, int _nbLi)
	{
		m_nbLi = _nbLi;
		m_nbCo = _nbCo;

		m_array = new T**[_nbCo];
		m_array[0] = new T*[_nbCo * _nbLi];

		// initialisation du 'tableau' indexeur
		for(int co = 0 ; co < m_nbCo ; co++) {
			m_array[co] = m_array[0] + co * _nbLi;
		}

		// initialisation a null de l'entièreté du tableau 2d
		memset(m_array[0], NULL, _nbLi * _nbCo * sizeof(T*));

		// sinon, le tableau est accessible comme ton tableau ***
		// exemple avec une init 'old school'
		for(int li = 0 ; li < _nbLi ; li++) {
			for(int co = 0 ; co < _nbCo ; co++) {
				m_array[co][li] = NULL;
			}
		}
	}

	void deallocate()
	{
		// Détruire tout le contenu
		T * toDestroy;
		for(int content = 0 ; content < m_nbLi * m_nbCo ; content++)
		{
			toDestroy = m_array[0][content];

			if(toDestroy != NULL)
			{
				delete toDestroy;
			}
		}

		delete [] m_array[0];

		delete [] m_array;
	}

};

#endif
