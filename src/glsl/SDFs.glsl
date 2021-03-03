#version 330 core
out vec4 outColor;

// Signed Distance Functions
// =========================

// Source of the following functions: http://iquilezles.org/www/articles/distfunctions/distfunctions.htm
// =====================================================================================================
// See the source for readable versions. I need to fit into a nice number of lines (see end of file)

float dot2( in vec2 v ) { return dot(v,v); } float dot2( in vec3 v ) { return dot(v,v); } float ndot( in vec2 a, in vec2 b ) { return a.x*b.x - a.y*b.y; }

// Distance Functions
// ==================
//                                                                                 // IMPLEMENTATIONS --->
float d2Sphere      (in vec3 pos, in vec3 sp, in float r)                           { return length(pos-sp)-r; }
// Cube on position sp, with scale b, smooth corner radius r
float d2Cube        (in vec3 pos, in vec3 sp, in vec3 b, in float r)                { vec3 p=pos-sp;vec3 q=abs(p)-b;return length(max(q,0.0))+min(max(q.x,max(q.y,q.z)),0.0)-r; }
// Box Frame on sp, scale b, and thickness of e
float d2BoxFrame    (vec3 pos, vec3 sp, vec3 b, float e)                            { vec3 p=pos-sp;p=abs(p)-b;vec3 q=abs(p+e)-e;return min(min(length(max(vec3(p.x,q.y,q.z),0.0))+min(max(p.x,max(q.y,q.z)),0.0),length(max(vec3(q.x,p.y,q.z),0.0))+min(max(q.x,max(p.y,q.z)),0.0)),length(max(vec3(q.x,q.y,p.z),0.0))+min(max(q.x,max(q.y,p.z)),0.0)); }
float d2Torus       (vec3 pos, vec3 sp, vec2 t)                                     { vec3 p=pos-sp;vec2 q=vec2(length(p.xz)-t.x,p.y);return length(q)-t.y; }
float d2CappedTorus (in vec3 pos, in vec3 sp, in vec2 sc, in float ra, in float rb) { vec3 p=pos-sp;p.x=abs(p.x);float k=(sc.y*p.x>sc.x*p.y)?dot(p.xy,sc):length(p.xy);return sqrt(dot(p,p)+ra*ra-2.0*ra*k)-rb; }
float d2Link        (vec3 pos, vec3 sp, float le, float r1, float r2)               { vec3 p=pos-sp;vec3 q=vec3(p.x,max(abs(p.y)-le,0.0),p.z);return length(vec2(length(q.xy)-r1,q.z))-r2; }
float sdPlane       (vec3 pos, vec3 sp, vec3 n, float h)                            { vec3 p=pos-sp;return dot(p,normalize(n))+h; }
float sdHexPrism    (vec3 pos, vec3 sp, vec2 h)                                     { vec3 p=pos-sp;const vec3 k=vec3(-0.8660254,0.5,0.57735);p=abs(p);p.xy-=2.0*min(dot(k.xy,p.xy),0.0)*k.xy;vec2 d=vec2(length(p.xy-vec2(clamp(p.x,-k.z*h.x,k.z*h.x),h.x))*sign(p.y-h.x),p.z-h.y);return min(max(d.x,d.y),0.0)+length(max(d,0.0)); }
float sdTriPrism    (vec3 pos, vec3 sp, vec2 h )                                    { vec3 p=pos-sp;vec3 q=abs(p);return max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5);}
float sdCapsule     (vec3 pos, vec3 sp, vec3 a, vec3 b, float r)                    { vec3 p=pos-sp;vec3 pa=p-a,ba=b-a;float h=clamp(dot(pa,ba)/dot(ba,ba),0.0,1.0);return length(pa-ba*h)-r; }
// Cylinder from a to b with radius r
float d2Cylinder    (in vec3 p, in vec3 a, in vec3 b, in float r)                   { vec3 ba=b-a;vec3 pa=p-a;float baba=dot(ba,ba);float paba=dot(pa,ba);float x=length(pa*baba-ba*paba)-r*baba;float y=abs(paba-baba*0.5)-baba*0.5;float x2=x*x;float y2=y*y*baba;float d=(max(x,y)<0.0)?-min(x2,y2):(((x>0.0)?x2:0.0)+((y>0.0)?y2:0.0));return sign(d)*sqrt(abs(d))/baba; }
float sdRoundCone   (vec3 pos, vec3 sp, vec3 a, vec3 b, float r1, float r2)         { vec3 p=pos-sp;vec3 ba=b-a;float l2=dot(ba,ba);float rr=r1-r2;float a2=l2-rr*rr;float il2=1.0/l2;vec3 pa=p-a;float y=dot(pa,ba);float z=y-l2;float x2=dot2(pa*l2-ba*y);float y2=y*y*l2;float z2=z*z*l2;float k=sign(rr)*rr*rr*x2;if(sign(z)*a2*z2>k)return sqrt(x2+z2)*il2-r2;if(sign(y)*a2*y2<k)return sqrt(x2+y2)*il2-r1;return(sqrt(x2*a2*il2)+y*rr)*il2-r1; }
float sdPyramid     (vec3 pos, vec3 sp, float h)                                    { vec3 p=pos-sp;float m2=h*h+0.25;p.xz=abs(p.xz);p.xz=(p.z>p.x)?p.zx:p.xz;p.xz-=0.5;vec3 q=vec3(p.z,h*p.y-0.5*p.x,h*p.x+0.5*p.y);float s=max(-q.x,0.0);float t=clamp((q.y-0.5*p.z)/(m2+0.25),0.0,1.0);float a=m2*(q.x+s)*(q.x+s)+q.y*q.y;float b=m2*(q.x+0.5*t)*(q.x+0.5*t)+(q.y-m2*t)*(q.y-m2*t);float d2=min(q.y,-q.x*m2-q.y*0.5)>0.0?0.0:min(a,b);return sqrt((d2+q.z*q.z)/m2)*sign(max(q.z,-p.y)); }
// not signed, triangle abc.
float udTriangle    (vec3 p, vec3 a, vec3 b, vec3 c)                                { vec3 ba=b-a;vec3 pa=p-a;vec3 cb=c-b;vec3 pb=p-b;vec3 ac=a-c;vec3 pc=p-c;vec3 nor=cross(ba,ac);return sqrt((sign(dot(cross(ba,nor),pa))+sign(dot(cross(cb,nor),pb))+sign(dot(cross(ac,nor),pc))<2.0)?min(min(dot2(ba*clamp(dot(ba,pa)/dot2(ba),0.0,1.0)-pa),dot2(cb*clamp(dot(cb,pb)/dot2(cb),0.0,1.0)-pb)),dot2(ac*clamp(dot(ac,pc)/dot2(ac),0.0,1.0)-pc)):dot(nor,pa)*dot(nor,pa)/dot2(nor)); }

// Shape Operations
// ================
float opSmoothUnion       (float d1, float d2, float k) { float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 ); return mix( d2,  d1, h ) - k*h*(1.0-h); }
float opSmoothSubtraction (float d1, float d2, float k) { float h = clamp( 0.5 - 0.5*(d2+d1)/k, 0.0, 1.0 ); return mix( d2, -d1, h ) + k*h*(1.0-h); }
float opSmoothIntersection(float d1, float d2, float k) { float h = clamp( 0.5 - 0.5*(d2-d1)/k, 0.0, 1.0 ); return mix( d2,  d1, h ) + k*h*(1.0-h); }








// Errors are usually in fragmentShader.glsl but since I am inserting this code at the start of a fragmentShader.glsl and
// *then* compiling, the line number are wrong. When this file is excatly 50 lines long, I can at least do a simple subtraction
// to get the real linue number in fragmentShader.glsl