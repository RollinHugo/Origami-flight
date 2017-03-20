// ============================================================
//                  TP ORIGAMI - SIRV 2016
// ============================================================

#include "basics.inc"
#include "textures.inc" 
#include "aori.inc"// pour le pliage
#include "asalle.inc" // pour le decor de la scène

#declare basseQualite = 0;// mettre à 1 pour déprécier les textures et le framerate (rendu beaucoup plus rapide)
// cela donnes quelques incohérences dans la cinématique, mais le rendu reste presque le même

#if (basseQualite = 1) // qualité minimale
    #declare nb = 6;
    #declare Textur1=texture {pigment{color Cyan}};
    #declare Textur2=Textur1;
#else
    #declare nb = 10;
    #declare Textur1 = texture{
      pigment{ color Cyan}
      normal { bumps 0.5 scale 0.05}
      finish { diffuse 0.9 phong 0}
    }
    #declare Textur2 = texture{
      pigment{ ripples scale 0.25 turbulence 1.5
        color_map { [0.0 color Cyan]
                    [0.05 color rgb<100/256,193/256,186/256>]
                    [0.6 color Cyan ]
                    [0.95 color  rgb<100/256,193/256,186/256>]
                     
            }
        }
     normal { wrinkles 0.5 scale 0.1 }
     finish { diffuse 0.3 phong 0}
     };
#end

#declare e=.02;  // epaisseur de la feuille de papier
#declare l=1; // demi longeur de la fauille de papier
              
// on rend la salle :
Salle(Textur1,Textur2)

// ===============================================================
//                           CINEMATIQUE
// ===============================================================
// position de la feuille en début de pliage :
#declare position_finale = <-0.70*cos(45)*0.5*15,-2*15*e*sin(0.5*7)-2*15*e+0.099,0.70*cos(45)*0.5*15>;  
// animations de la feuille, en fonction de la valeur de clock :
#switch(clock)
    // DEPLACEMENT DE LA FEUILLE DU TAS VERS LA ZONE DE PLIAGE : 
    // un déplacement sinusoïdal en 3 sous-phases :
    #range (0,15)
        box { <-1.00, 0.00, -1.00>,< 1.00, 2.00, 1.00>   
              texture{Textur1}
              texture{Textur2} 
              scale <l,e*0.5,l> translate<0,15*e-1.75,-4> 
              #if (clock<8)
                translate<-0.70*cos(45)*0.5*clock,2*15*e*sin(0.5*0.5*clock),0.70*cos(45)*0.5*clock>
              #end  
              #if(clock=8)
                translate<-0.70*cos(45)*0.5*8,2*15*e*sin(0.5*0.5*8),0.70*cos(45)*0.5*8>
              #end
              #if (clock>8&clock<15)
                translate<-0.70*cos(45)*clock*0.5,l*clock*(1/14),0.70*cos(45)*0.5*clock>
              #end
        }
    #break
    // FORMATION DE L'ORIGAMI :
    // MAX étant défini globalement dans le fichier annexe (nombre d'étapes de pliage)
    #range(15,MAX*nb+16)
        ori(clock-15,Textur1,Textur2, transform{translate<0,15*e-1.75+2*l,-4> translate position_finale}, nb)
    #break
    // DEMI - TOUR POUR S'ORIENTER VERS LA FENETRE :
    #range(MAX*nb+16, MAX*nb+16+nb)
        #local valu=180*(clock-MAX*nb-16)/nb;
        ori(MAX*nb, Textur1,Textur2, transform{rotate <0,valu,0> translate<0,15*e-1.75+2*l,-4> translate position_finale }, nb)
    #break
    // VOL !
    #else
        #local valu=clock-MAX*nb-16-nb; // frame de l'animation
        #local dep = -<valu,sin(valu*.5)-valu/5,-valu/5>*.2; // vecteur de déplacement
        ori_deplacement(clock-15, Textur1,Textur2, transform{rotate <0,180,0> translate<0,15*e-1.75+2*l,-4> translate position_finale translate dep}, nb)
#end
 
 
// Choix de la vue
#declare distance_camera =15;
// renvoie un vecteur en rotation autour de 0, à la distance d, la fréquence/vitesse f, un angle phi et à la frame cl 
#macro rotCam(d,f,phi,cl)
    d*<sin(f*cl+phi),0,cos(f*cl+phi)>
#end

// on choisit le placement de la caméra selon l'horologe :
camera { 
    #local posOri=position_finale + <0,15*e-1.75+l,-4>;
    #switch(clock)
        #range(0,50)
            location posOri + rotCam(10,.005,pi/4,clock) + <0,.2,0>
            look_at posOri 
            #break
        #range(50,100)
            #local d=10 - (10-5)*((clock-50)/50);
            location posOri + rotCam(d,.005,pi/4,50)+<0,.2,0>
            look_at posOri 
            #break
        #range(100,150)
            location posOri + rotCam(5,.005,pi/4,150-clock)+<0,.2,0>
            look_at posOri
            #break
        #range(150,200)
            #local d=5 + (10-5)*((clock-150)/50);
            location posOri + rotCam(d,.005,pi/4,0)+<0,.2,0>
            look_at posOri
            #break
        #range(200,250)
            location posOri + rotCam(10,.005,pi/4,clock-200)+<0,.2,0>
            look_at posOri
            #break
        #range(250,300)
            location posOri + rotCam(10,.005,pi/4,300-clock)+<0,.2,0>
            look_at posOri
            #break
        #else
            location posOri +rotCam(10,.005,pi/4,0)+<0,.2,0>
            look_at posOri
    #end
}

