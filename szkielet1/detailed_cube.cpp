/*
Niniejszy program jest wolnym oprogramowaniem; mo¿esz go
rozprowadzaæ dalej i / lub modyfikowaæ na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacjê Wolnego
Oprogramowania - wed³ug wersji 2 tej Licencji lub(wed³ug twojego
wyboru) którejœ z póŸniejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej¹, i¿ bêdzie on
u¿yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyœlnej
gwarancji PRZYDATNOŒCI HANDLOWEJ albo PRZYDATNOŒCI DO OKREŒLONYCH
ZASTOSOWAÑ.W celu uzyskania bli¿szych informacji siêgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnoœci¹ wraz z niniejszym programem otrzyma³eœ te¿ egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeœli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#include "detailed_cube.h"


namespace Models {
	
	
	DetailedCube detailedCube;

	DetailedCube::DetailedCube() {
		initCube();
	}
	
	
	
	DetailedCube::~DetailedCube() {		
		freeCube();
	}


	void DetailedCube::quad(int subdiv,int i1, int i2, float x, float y, float back, float nx, 
		float ny, float s,float t,int pos){
			vertices[i1*subdiv*3*4+i2*3*4+0+pos*3]=x;
			vertices[i1*subdiv*3*4+i2*3*4+1+pos*3]=y;
			vertices[i1*subdiv*3*4+i2*3*4+2+pos*3]=-back;
			normals[i1*subdiv*3*4+i2*3*4+0+pos*3]=nx;
			normals[i1*subdiv*3*4+i2*3*4+1+pos*3]=ny;
			normals[i1*subdiv*3*4+i2*3*4+2+pos*3]=-1.0f/3;
			texCoords[i1*subdiv*2*4+i2*2*4+0+pos*2]=s;
			texCoords[i1*subdiv*2*4+i2*2*4+1+pos*2]=t;
	}

	void DetailedCube::initCube() {
		int subdiv=100; float back=1;
		float dn=(2.0f/3)/subdiv;
		float nx=-1.0f/3; float ny=-1.0f/3;
		float s=0; float t=0;
		float dst=1.0f/subdiv;
		float x=-back; float y=-back;
		float dp=(float)2*back/subdiv;

		vertices=new float[4*3*subdiv*subdiv];texCoords=new float[4*2*subdiv*subdiv];
		normals=new float[4*3*subdiv*subdiv];vertexCount=4*subdiv*subdiv;
		for (int i1=0;i1<subdiv;i1++) {
			for (int i2=0;i2<subdiv;i2++) {
				quad(subdiv,i1,i2,x,y,back,nx,ny,s,t,0);
				quad(subdiv,i1,i2,x+dp,y,back,nx+dn,ny,s+dst,t,1);
				quad(subdiv,i1,i2,x+dp,y+dp,back,nx+dn,ny+dn,s+dst,t+dst,2);
				quad(subdiv,i1,i2,x,y+dp,back,nx,ny+dn,s,t+dst,3);
				nx+=dn;x+=dp;s+=dst;
			}
			nx=-1.0f/3;x=-back;s=0;
			ny+=dn;y+=dp;t+=dst;
		}
	}

	void DetailedCube::wall() {
		glEnable(GL_NORMALIZE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3,GL_FLOAT,0,vertices);
		glNormalPointer(GL_FLOAT,0,normals);
		glTexCoordPointer(2,GL_FLOAT,0,texCoords);
		glDrawArrays(GL_QUADS,0,vertexCount);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	void DetailedCube::drawSolid() {
		glPushMatrix();
		wall();
		glRotatef(90,1,0,0);
		wall();
		glRotatef(90,1,0,0);
		wall();
		glRotatef(90,1,0,0);
		wall();
		glRotatef(90,1,0,0);
		glRotatef(90,0,1,0);
		wall();
		glRotatef(180,0,1,0);
		wall();
		glPopMatrix();
	}

	void DetailedCube::freeCube() {
		delete []vertices;
		delete []texCoords;
		delete []normals;
	}

}