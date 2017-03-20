// Variables globales :
#declare e=.02;                 // épaisseur de la feuille
#declare l=1;                   // demi-longueur de la feuille
#declare MAX=20;                // nombre d'étapes de formation
// Paramètre de batement des ailes :
#declare vitesse=10;            // fréquence
#declare a0=-20;                // angle minimal (par rapport à une feuille non pliée)
#declare a1=20;                 // angle maximal

#declare null=transform {};

// ----------------------------------------------------------------------
//                                TEST   
// teste le bon fonctionement du fichier
// paramètres : horloge secondaire
// ----------------------------------------------------------------------

#macro test(clock2)
    #local tex=texture { pigment{ color rgb<.7,.7, 1.00>}};
    #if (clock2<MAX*10+1)
        ori(clock2,tex,tex , transform {scale 1.5},10)
    #else
        ori_deplacement(clock,tex, tex, transform {scale 1.5},10,)
    #end
#end

// ----------------------------------------------------------------------
//                             DEPLACEMENT
// gère le batement des ailes lors d'un mouvement
// paramètres :
//      - horloge secondaire
//      - textures 1 et 2 
//      - transformations
//      - nombre de frames par étape
// ----------------------------------------------------------------------

#macro ori_deplacement(clock2,tex1,tex2,trans,nb)
    #local anglE=(a1+a0)/2+(a1-a0)*cos(2*pi*clock2/vitesse);
    ori_angle(tex1,tex2,trans,nb,anglE)
#end

// gère l'angle des ailes
// paramètres :
//      - Textures 1 et 2
//      - transformations
//      - nombre de frame par étapes
//      - angle (0 étant une feuille plane) 


#macro ori_angle(tex1, tex2, trans, nb,a)
    #local ori1 = object { ori(MAX*nb+1, tex1,tex2,null,nb)}; // origami sans ailes non transformé
    // on forme les ailes avec une boite comme suit :
    #local h=.4;
    #local h0=.34;
    #local boite= box {
         <-10,-10,-h>
         <10,10,10>
    }
    // on gère les eux ailes séparement et on retransforme à la fin :
    union{
        difference {
            object {ori1}
            object {boite}
            translate <0,-h0,h>
            rotate <a,0,0>
            translate <0,h0,-h> 
        }
        difference {
            object {ori1}
            object {boite rotate <180,0,0>}
            translate <0,-h0,-h>
            rotate <-a,0,0>
            translate <0,h0,h>   
        }
        intersection{
            object {ori1}
            box{<-10,-10,-h><10,10,h>}
        }
        transform trans 
    }
#end

// ----------------------------------------------------------------------
//                            CONSTRUCTION
// On va construire étape par étape le modèle. La complexité augmente au fur et à mesure car il faut souvent regénérer les n premières frames pour faire la n+1 eme.
// paramètres :
    // - horloge secondaire
    // - textures 1 et 2
    // - transformations globales
    // - nombre de frames par étapes
// ----------------------------------------------------------------------


#macro ori(clock2, tex1, tex2, trans, nb)
    // voir schéma. La longeur est données par des considérations géométriques
    #local cc=l/sqrt(2)*(1-tan(pi/8));
    
    // des angles, aussi calculés géométriquement
    #local alpha3=atan(1-sqrt(2)*cc/l)/2;
    #local xalpha3=l*tan(alpha3);        
    #local beta3=atan(cc/(sqrt(2)*l-cc))/2; 
    #local xbeta3=(sqrt(2)*l-cc)*tan(beta3);
    
    // un triangle subdivisé, au total 1/8 du carré, voir schéma
    // on repositionne/tourne les pièces avant d'appliquer les transformations
    #macro demicoin(tt,tt1,tt2,tt3 ,tt4,tt5,tt6,tt7)                 
        union{  
            union{
                union{                                          // ZONE 1
                     prism{0,e,4                                    // ZONE 4 
                        <0,0>
                        <l,0>
                        <l,xalpha3>
                        <0,0>
                        translate <0,-e/2,0>
                        rotate <0,alpha3,0>*180/pi
                        transform tt4
                        rotate <0,-alpha3,0>*180/pi
                     }
                      prism{0,e,4                                    // ZONE 5
                        <0,0>
                        <l,xalpha3>
                        <l,l-sqrt(2)*cc>
                        <0,0>
                        translate <0,-e/2,0>
                        rotate <0,alpha3,0>*180/pi 
                        transform tt5
                        rotate <0,-alpha3,0>*180/pi
                     }
                     transform tt1
                }
                union{                                          // ZONE 2
                
                    prism {0,e,4                                    // ZONE 6
                        <0,0>
                        <l,l-sqrt(2)*cc>
                        <l-(cc-xbeta3)/sqrt(2),l-(cc+ xbeta3)/sqrt(2)>
                        <0,0>
                        translate <0,-e/2,0>
                        rotate <0,45-beta3*180/pi,0>
                        transform tt6
                        rotate -<0,45-beta3*180/pi,0>
                    }                                              
                    prism {0,e,4                                   // ZONE .
                        <0,0>
                        <l-(cc-xbeta3)/sqrt(2),l-(cc+ xbeta3)/sqrt(2)>
                        <1,1>/sqrt(2)*(sqrt(2)*l-cc)
                        <0,0>
                        translate <0,-e/2,0>
                    }        
                } 
                transform tt2 
            }                                                   // ZONE 3
            union{                                                 // ZONE 7
                prism {0,e,4
                    <cc,0>
                    <xalpha3,0>
                    <xalpha3,cc-xalpha3>
                    <cc,0>
                    translate <0,-e/2,0>
                    translate <-xalpha3,0,0>
                    transform tt7
                    translate <xalpha3,0,0>    
                }
                
                prism {0,e,5
                    <0,0>
                    <0,cc>
                    <xalpha3,cc-xalpha3>
                    <xalpha3,0>
                    <0,0>
                    translate <0,-e/2,0>
                    }
                
                rotate <0,45,0>
                transform tt3
                translate <1,0,1>*(l-cc/sqrt(2)) 
                
            }
            texture {tex1}
            texture {tex2}
            transform tt
        }  
    #end
    
    // fonctions de type barycentre :
    // paramètres : Valeur de début, valeur finale, temps de parcours, temps présent
    #local allerRetour=function(x, y, z, k1){ ( x+(y-x)*k1*2/z )*(k1<=z/2) + ( y-(y-x)*(k1-z/2)/z*2 )*(k1>z/2) }
    #local allerSimple=function(x, y, z, k1){ x+(y-x)*k1/z }
    // idem, mais prenant en compte dses valeurs hors des bornes
    #local allerSimple2=function(x, y, z, k1, k2){(z<=k1 & k1<k2) *(x+(y-x)*(k1-z)/(k2-z)) +(k1>=k2)*y}
    #local allerRetour2=function(x, y, z, k1, k2){ (k1<=z+(k2-z)/2)*allerSimple2(x,y,z,k1,(k2-z)/2+z) + (k1>z+(k2-z)/2)*allerSimple2(y,x,(k2-z)/2+z,k1,k2) }
                                                             
    // TRANSFORMATIONS :
    // une serie de transformations. Chaque <,,> représente une rotation pour un des 8 sous-triangles du carré 
    #local p0 = array[8]              {<0,0,0>,<2,0,0>,<1,1,1>,<-1,1,1>,<2,0,2>,<0,0,2>,<0,-1,0>,<2,-1,0>} // positionnement initial 
    #local tournerGauche =array [8]   {<0,0,1>,<0,0,1>,<0,0,1>,<0,0,0>,<0,0,0>,<0,0,0>,<0,0,0>,<0,0,1>}  
    #local tournerBas =array [8]      {<0,0,0>,<1,0,0>,<1,0,0>,<1,0,0>,<1,0,0>,<0,0,0>,<0,0,0>,<0,0,0>}
    #local retournerFeuille =array[8] {<1,0,0>,<1,0,0>,<1,0,0>,<1,0,0>,<1,0,0>,<1,0,0>,<1,0,0>,<1,0,0>}
    #local pivoter =array[8]          {<0,1,0>,<0,1,0>,<0,1,0>,<0,1,0>,<0,1,0>,<0,1,0>,<0,1,0>,<0,1,0>}
    #local tournerDiag1=array[8]      {<1,0,0>,<1,0,0>,<1,0,0>,<1,0,0>,<0,0,0>,<0,0,0>,<0,0,0>,<0,0,0>}
    #local tournerDiag2=array[8]      {<0,0,0>,<0,0,0>,<0,0,-1>,<0,0,-1>,<0,0,-1>,<0,0,-1>,<0,0,0>,<0,0,0>}
   
    // on initialise les tableaux des transformations des sous-parties des triangles
    #local t0=array[8] { transform {}, transform {}, transform {}, transform {}, transform {}, transform {}, transform {}, transform {}};
    #local t1=t0;
    #local t2=t0;
    #local t3=t0; 
    #local t4=t0;
    #local t5=t0;
    #local t6=t0;
    #local t7=t0;
    
    // PREPARATION DE LA FEUILLE
    // quelques étapes préliminaires ( pliage!/dépliage...)
    #local lim=6*nb+1; // frame de fin de la préparation
    #if(clock2<lim) 
        union{
            #for (i,0,7)
                // transformation globale
                #local t0[i]=transform{
                    rotate p0[i]*90
                    rotate (clock2<=nb)               * tournerBas[i]      * allerRetour(0,180,nb,clock2)
                    rotate (clock2>nb & clock2<=2*nb)  * tournerGauche[i]   * allerRetour(0,180,nb,clock2-nb)
                    rotate (clock2>2*nb & clock2<=3*nb)* retournerFeuille[i]* allerSimple(0,180,nb,clock2-2*nb)
                    rotate (clock2>3*nb & clock2<=5*nb)* pivoter[i]         * 45
                    rotate (clock2>3*nb & clock2<=4*nb)* tournerDiag1[i]    * allerRetour(0,180,nb,clock2-3*nb)
                    rotate (clock2>4*nb & clock2<=5*nb)* tournerDiag2[i]    * allerRetour(0,180,nb,clock2-4*nb)
                    rotate (clock2>3*nb & clock2<=5*nb)* pivoter[i]         * -45
                    rotate (clock2>5*nb & clock2<=6*nb)* retournerFeuille[i]* allerSimple(0,90,nb,clock2-5*nb)
                    translate <0,-l,0>
                }
                demicoin(t0[i],null,null,null,null,null,null,null) 
            #end
            transform trans // transformation héritée de la scène principlale
        }
    
    // PLIAGE REEL      
    // étape la plus longue. On répète chacune des étapes précédante pour calculer une frame
    // -  pour chaque triangle, on calcule sur chacune de ses sous-partie les rotations et translations nécessaire, puis on les applique
    // -  lorsque les transformations 3D sont trop compliquées à calculer, on les approche par les focnctions affines par morceaux calculées expérimentalement
    // -  il est très dur de faire une rotation autour d'un axe quelquonque en povRay, car par exempel :
    //        rotate<x,y,0> == rotate<x,0,0> rotate<y,0,0> != rotate<0,y,0> rotate<x,0,0>   
    //        donc souvent, on décomposera par rapport aux axes en plusieurs transformations plus simples, mais moins lisibles
    // - Pour chaque étape, la forme des rotations est obtenue par l'expérience et le calcul
    // - Pour plus de lisibilité, les rotations sont exprimées divisées par 45 ou par 90° :w
    #else
        // ETOILAGE :
        // on approche par morceau la transformation :
        #local donnees=array[11]{0,44,31,25,21.3,19,17.5,17.9,17.9,2,2};// trouvé par l'expérience
        #macro baisse(aaa) // valeur de la fonction approchée pour une valeur d'angle principal aaa
            #local ind=floor(.1*aaa);
            aaa*.1*(donnees[ind]+(donnees[ind+1]-donnees[ind])*mod(aaa,10)/10)
        #end
        #local etoile1 =array[8] {<0,-1,0>,<0,-1,0>,<0,0,0>,<0,0,0>,<0,1,0>,<0,1,0>,<0,0,0>,<0,0,0>};// placement plan
        #local etoile2 =array[8] {<0,0,0>,<0,0,0>,<-1,0,0>,<0,0,0>,<0,0,0>,<0,0,0>,<0,0,0>,<1,0,0>};// plancement ortho 
        #local etoile2b=array[8] {<0,0,0>,<0,0,0>,<0,0,0>,<-1,0,0>,<0,0,0>,<0,0,0>,<1,0,0>,<0,0,0>};// plancement ortho
        #local etoile3 =array[8] {<0,0,0>,<0,0,0>,<0,-1,0>,<0,1,0>,<0,0,0>,<0,0,0>,<0,1,0>,<0,-1,0>};// deplacement ortho    
        #macro etoile(i,dep,fin)
            // transformation du triangle 'i' par étoilage entre les frames 'dep' et 'fin'
            transform{
                rotate etoile1[i] *allerSimple2(0,45,dep,clock2-lim,fin)
                rotate pivoter[i] *45
                rotate etoile2[i] *baisse(allerSimple2(0,45,dep,clock2-lim,fin))
                rotate pivoter[i] *-90
                rotate etoile2b[i]*baisse(allerSimple2(0,45,dep,clock2-lim,fin)) 
                rotate pivoter[i] *45
                rotate etoile3[i] *allerSimple2(0,45,dep,clock2-lim,fin)    
            }
        #end
        
        // ECRASAGE :
        #local ecrasage=array[8] {<0,0,0>,<0,0,0>,<0,0,1>,<0,0,1>,<0,0,0>,<0,0,0>,<0,0,1>,<0,0,1>};
        // ECAILLAGE :
        #local ecaillageT1=array[8] {<-1,0,0>,<-1,0,0>,<1,0,0>,<1,0,0>,<-1,0,0>,<1,0,0>,<-1,0,0>,<1,0,0>};
        #local ecaillageT3=array[8] {<1,0,0>,<1,0,0>,<-1,0,0>,<-1,0,0>,<-1,0,0>,<-1,0,0>,<1,0,0>,<1,0,0>};
        // OUVERTURE :
        #local ouvertureT2=array[8] {<0,0,0>,<-1,0,0>,<0,0,0>,<1,0,0>,<-1,0,0>,<0,0,0>,<1,0,0>,<0,0,0>};
        #local ouvertureT1=array[8] {<-1,0,0>,<1,0,0>,<1,0,0>,<-1,0,0>,<1,0,0>,<-1,0,0>,<-1,0,0>,<1,0,0>};
        // on approche le déplacement par une fonction linéaire par morceau : (pas de 10, fait à la main, en %)
        #local approxOuv=array[20] {0,30,36,38,40,41,42,44,46,48,50,53,57,62,68,77,90,95,100,100};
        #macro alpha2(aaa)
                .01*aaa*(approxOuv[floor(.1*aaa)]+(approxOuv[floor(aaa/10)+1]-approxOuv[floor(aaa/10)])*mod(aaa,10)/10)
        #end
        // PLIAGE DES BORDS EXETRIEURS
        #local pliageex  =array[8] {<-1,0,0>,<0,0,0>,<1,0,0>,<0,0,0>,<0,0,0>,<-1,0,0>,<0,0,0>,<1,0,0>};                                        
        #local pliageex2 =array[8] {<0,0,1>,<0,0,0>,<0,0,-1>,<0,0,0>,<0,0,0>,<0,0,1>,<0,0,0>,<0,0,-1>};                                         
        // MISE A PLAT
        #local map      =array[8] {<0,0,1>,<0,0,1>,<0,0,1>,<0,0,1>,<0,0,-1>,<0,0,-1>,<0,0,-1>,<0,0,-1>};
        
        // QUEUE + TETE
        #local tq2      =array[8] {<0,1,0>,<0,0,0>,-<0,1,0>,<0,0,0>,<0,0,0>,-<0,1,0>,<0,0,0>,<0,1,0>};
        #local tq3      =array[8] {<0,0,0>,<0,0,1>,<0,0,0>,<0,0,1>,<0,0,-1>,<0,0,0>,<0,0,-1>,<0,0,0>};
        #local tq4      =array[8] {<0,0,1>,<0,0,0>,<0,0,1>,<0,0,0>,<0,0,0>,<0,0,-1>,<0,0,0>,<0,0,-1> };
        
        // on calcule toute la feuille de papier, mais on le rend pas 
        #local un =union{
            #for (i,0,7)
                // trans. gros triangle
                #local t0[i]=transform{
                    rotate p0[i]*90
                    rotate tournerBas[i]*allerSimple2(0,180,0,clock2-lim,nb)
                    etoile(i,nb,2*nb)
                    rotate ecrasage[i]*allerSimple2(0,90,2*nb,clock2-lim,3*nb)               
                    rotate map[i]*allerSimple2(0,90,8*nb,clock2-lim,9*nb)
                    rotate -map[i]*allerSimple2(0,90,10*nb,clock2-lim,11*nb)
                    translate <0,0,-l>
                    rotate tq2[i]*allerSimple2(0,50,11*nb,clock2-lim,12*nb) 
                    rotate <-90,0,0>
                }
                // trans. pointe
                #local t3[i]=transform{
                    rotate <0,-45,0>
                    rotate ecaillageT3[i]*allerRetour2(0,180,3*nb,clock2-lim,4*nb)
                    rotate ecaillageT3[i]*allerRetour2(0,-180,4*nb,clock2-lim,5*nb)
                    rotate <0,45,0>
                }
                // trans bord
                #local t1[i]=transform{ 
                    rotate <0,180/pi*atan(l-sqrt(2)*cc),0>
                    rotate ecaillageT1[i]*allerRetour2(0,180,5*nb,clock2-lim,6*nb)
                    rotate ouvertureT1[i]*alpha2(allerSimple2(0,180,6*nb,clock2-lim,7*nb))
                    rotate -<0,180/pi*atan(l-sqrt(2)*cc),0>
                }
                // trans cote
                
                #local t2[i]=transform{
                    translate -(l-sqrt(2)*cc/2)*<1,0,1>
                    rotate <0,-45,0>
                    rotate ouvertureT2[i]*allerSimple2(0,180,6*nb,clock2-lim,7*nb)
                    rotate <0,45,0>
                    translate (l-sqrt(2)*cc/2)*<1,0,1>
    
                    rotate <0,45,0>
                    translate <-l,0,0>
                    rotate tq4[i]*allerSimple2(0,180,9*nb,clock2-lim,10*nb)
                    rotate tq3[i]*allerSimple2(0,50,12*nb,clock2-lim,13*nb) 
                    translate <l,0,0>
                    rotate <0,-45,0> 
          
                }
                #local t5[i]=transform{
                    rotate pliageex[i]*allerSimple2(0,180,7*nb,clock2-lim,8*nb)
                }
                #local t6[i]=transform{
                    rotate pliageex[i]*allerSimple2(0,180,7*nb,clock2-lim,8*nb)
                }
                #local t7[i]=transform{ 
                    rotate pliageex2[i]*allerSimple2(0,180,7*nb,clock2-lim,8*nb)
                }
                demicoin(t0[i],t1[i],t2[i],t3[i],t4[i],t5[i],t6[i],t7[i])    
            #end
        }
        // SI le pliage n'est pas pas terminé, on affiche l'object calculé avec les ransformations issues de la scène principale
        #if (clock2<19*nb+1)
            object {un transform trans} 
        // SINON, on plie le nez :
        // comme la méthode avec les sous-triangles commence à montrer des limites de complexités, on va couper l'oiseau avec une sphèe pour faire le bec.

        #else
            #local spher=sphere{<.6,.6,0> .2}
            union{
                intersection{
                    object {spher}
                    object {un}
                    translate <-.6,-.4,0>
                    rotate <0,0,-1>*allerSimple2(0,90,19*nb, clock2,20*nb)
                    translate <.51,.36,0>
                }
                difference{
                    object {un}
                    object {spher}
                }
                transform {trans} 
            }
        #end
        
    #end
#end
