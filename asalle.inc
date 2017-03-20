#include "basics.inc"  
#include "textures.inc" 

// la macro principale du fichier
#macro Salle(Textur1 Textur2)
    // sources de lumière 
    union{
        light_source {
            <-10, 1.5, 0>
            color White
        }
        light_source{
            <0,2,0>
            color Gray50
        } 
         light_source{ <1,20,1>
        }
    }
    
    // ciel    
    sky_sphere{pigment{color rgb <.7,.7,1>}}

    //salle + fenêtre
    union
        { difference
       { box { <-11, -7, -11>, <16, 7, 10.5> } 
         box { <-10, -6, -10>, <15, 6, 10> }
         box { <-4, -2, 9.9>, <2, 3, 10.6>}
        texture{ pigment{color rgb<143/256,194/256,245/256>} }
       }
       union{
       box { <-1.25, -2, 10>, <-0.75, 3, 10.5> }
       box { <-4, 0.25, 10>, <2, 0.75, 10.5> }
       texture{Silver_Metal} 
       }
      rotate<0,-90,0>
       }
     
    //tableau
    box { <3-5, 0, -9.99>,< 3-5+2, 2.00, -9.99>   
        texture { pigment{
            image_map{ png "img.png"
                map_type 0 
                interpolate 2
                once    
          }
          rotate<0,180,0>
          scale<2,2,0>}
        }
        scale <2,2,1> rotate<0,0,0> translate<0,-1,0> 
    }

    // textures de la table :
    #local Pigment_Pine_Wood3 = 
         pigment{ wood turbulence 0.02 octaves 4 lambda 3
                  scale 0.175 rotate <1, 1, 0> 
                  color_map {
                  [0.0 color rgb <1.30, 0.99, 0.64>]
                  [0.5 color rgb <0.75, 0.39, 0.17>]
                  [0.7 color rgb <0.75, 0.39, 0.17>]
                  [1.0 color rgb <1.30, 0.99, 0.64>]
                  }
                } 
    #local Pine_Wood_Bord_Texture =
          texture{ pigment{ Pigment_Pine_Wood3  }
                   normal { pigment_pattern{  Pigment_Pine_Wood3 }
                            0.25
                            translate<0,0,0>
                          }
                   finish { phong 0.3  phong_size 5 }
                   scale 0.2
                   translate <0.16,0.025,0.2>
                 }

    // pied de table à afficher à la position 'pos'
    #macro Pied(pos)
    box { <-1, 0, -1>,<1,2,1>
          scale <0.2,2,0.2> translate pos
        } 
    #end
     
    // table
    union { 
        box { <0.00, 0.00, 0.00>,< 0.30, 0.05, 1.00>   
              scale<9,5,5>  translate<-1.35,-e+1.9625,-2.5> 
        }// tablier
        Pied(<1.15 ,-e-2.05,2.3>)
        Pied(<-1.15,-e-2.05,-2.3>)
        Pied(<1.15 ,-e-2.05,-2.3>)
        Pied(<-1.15,-e-2.05,2.3>)
        texture{ Pine_Wood_Bord_Texture }
        rotate<0,-45,0> translate<-2.5*cos(45)+0.5,-3.99,-2.5*cos(45)-0.5>
        scale<1.5,1,1.5>
     } 

    // TAS DE PAPIER
    #local ecart=.08;// ecartage entre deux feuilles du tas 
    #local modulo=9;// nombres de modes de feuilles
    union {
        #for (i, 0, 14, 1)
            box { <-1.00, 0.00, -1.00>,< 1.00, 2.00, 1.00>   
                texture{Textur1}
                texture{Textur2} 
                scale <l,e*0.5,l> translate<0,i*e,0>
                translate <mod(i,modulo)-(modulo-1)/2,0,mod(i+1,modulo)-(modulo-1)/2>*ecart/modulo*2 // on randomize la position
            }
        #end 
    translate<0,-1.75,-4> 
    } 
#end

