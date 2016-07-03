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

#include "torus.h"


namespace Models {
	
	Torus torus;

	Torus::Torus() {
		buildTorus(0.75,0.25,20,20);
	}
	
	Torus::Torus(float R,float r,float mainDivs,float tubeDivs) {
		buildTorus(R,r,mainDivs,tubeDivs);
	}
	
	Torus::~Torus() {		
	}
	
	inline float Torus::d2r(float deg) {
		return PI*deg/180.0f;
	}

	vec4 Torus::generateTorusPoint(float R,float r,float alpha,float beta) {
		alpha=d2r(alpha);
		beta=d2r(beta);
		return vec4((R+r*cos(alpha))*cos(beta),(R+r*cos(alpha))*sin(beta),r*sin(alpha),1.0f);
	}

	vec4 Torus::computeVertexNormal(float alpha,float beta) {
		alpha=d2r(alpha);
		beta=d2r(beta);
		return vec4(cos(alpha)*cos(beta),cos(alpha)*sin(beta),sin(alpha),0.0f);
	}

	vec4 Torus::computeFaceNormal(vector<vec4> &face) {
		vec3 a=vec3(face[1]-face[0]);
		vec3 b=vec3(face[2]-face[0]);
		
		return normalize(vec4(cross(b,a),0.0f));
	}

	void Torus::generateTorusFace(vector<vec4> &vertices, vector<vec4> &vertexNormals, vec4 &faceNormal,float R,float r,float alpha,float beta,float step_alpha,float step_beta) {
		vertices.clear();
		vertexNormals.clear();	
		
		vertices.push_back(generateTorusPoint(R,r,alpha,beta));
		vertices.push_back(generateTorusPoint(R,r,alpha+step_alpha,beta));
		vertices.push_back(generateTorusPoint(R,r,alpha+step_alpha,beta+step_beta));
		vertices.push_back(generateTorusPoint(R,r,alpha,beta+step_beta));
		
		faceNormal=computeFaceNormal(vertices);
		
		vertexNormals.push_back(computeVertexNormal(alpha,beta));
		vertexNormals.push_back(computeVertexNormal(alpha+step_alpha,beta));
		vertexNormals.push_back(computeVertexNormal(alpha+step_alpha,beta+step_beta));
		vertexNormals.push_back(computeVertexNormal(alpha,beta+step_beta));
	}



	void Torus::buildTorus(float R,float r,float mainDivs,float tubeDivs) {
		vector<vec4> face;
		vector<vec4> faceVertexNormals;
		vec4 normal;

		internalVertices.clear();
		internalFaceNormals.clear();
		internalVertexNormals.clear();
			
		float mult_alpha=360.0f/tubeDivs;
		float mult_beta=360.0f/mainDivs;
		
		for (int alpha=0;alpha<round(tubeDivs);alpha++) {			
			for (int beta=0;beta<round(mainDivs);beta++) {
				
				generateTorusFace(face,faceVertexNormals,normal,R,r,alpha*mult_alpha,beta*mult_beta,mult_alpha, mult_beta);			
				
				internalVertices.push_back(face[0]);
				internalVertices.push_back(face[1]);
				internalVertices.push_back(face[2]);
				
				internalVertices.push_back(face[0]);
				internalVertices.push_back(face[2]);
				internalVertices.push_back(face[3]);
				
				internalVertexNormals.push_back(faceVertexNormals[0]);
				internalVertexNormals.push_back(faceVertexNormals[1]);
				internalVertexNormals.push_back(faceVertexNormals[2]);
				
				internalVertexNormals.push_back(faceVertexNormals[0]);
				internalVertexNormals.push_back(faceVertexNormals[2]);
				internalVertexNormals.push_back(faceVertexNormals[3]);
							
				for (int i=0;i<6;i++) internalFaceNormals.push_back(normal);
				
				
			}
		}
		
		vertices=(float*)internalVertices.data();
		normals=(float*)internalFaceNormals.data();
		vertexNormals=(float*)internalVertexNormals.data();
		texCoords=vertexNormals;
		vertexCount=internalVertices.size();
	}

	void Torus::drawSolid() {

		glEnableClientState(GL_VERTEX_ARRAY);	
		glEnableClientState(GL_NORMAL_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(4,GL_FLOAT,0,vertices);
		//glColorPointer(4,GL_FLOAT,0,colors);
		glNormalPointer(GL_FLOAT,sizeof(float)*4,vertexNormals);
		glTexCoordPointer(2,GL_FLOAT,0,texCoords);
		
		glDrawArrays(GL_TRIANGLES,0,vertexCount);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}


}
