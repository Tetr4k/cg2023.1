#include "colors.inc"
camera{
    location <6, 6, 6>
    look_at <0, 0, 0>
    angle 0
}
light_source{ <500, 500, -1000> White }
background { color White }

union{
    union{
        sphere{<0, 2, 0>, 1}
        pigment{ Yellow }
    }
    union{
        box{<-1,-1,-1>, <1,1,1>}
        pigment{ Red }
    }
    union{                 
        cylinder{ <1.5, 1, 0>, <1.5, -2, 0>, 0.5}  
        cylinder{ <-1.5, 1, 0>, <-1.5, -2, 0>, 0.5}
        cylinder{ <-1, -1, 0>, <-1, -5, 0>, 0.5}   
        cylinder{ <1, -1, 0>, <1, -5, 0>, 0.5}                    
        pigment{ Blue }
    }                       
}
