xof 0303txt 0032

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000,-0.000000, 1.000000, 0.000000,
     0.000000, 1.000000, 0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame TopWater {
    FrameTransformMatrix {
      -0.000062,-378.260315, 0.000000, 0.000000,
      378.260315,-0.000062,-0.000017, 0.000000,
       0.000017,-0.000000,378.260315, 0.000000,
       0.000015, 0.000004, 0.000000, 1.000000;;
    }
    Mesh { // TopWater mesh
      4;
      -10.536143;-11.155689;-0.022676;,
      10.963412;-11.155689;-0.022676;,
      10.963412;10.343866;-0.022676;,
      -10.536143;10.343866;-0.022676;;
      1;
      4;3,2,1,0;;
      MeshNormals { // TopWater normals
        4;
         0.000000; 0.000000; 1.000000;,
         0.000000; 0.000000; 1.000000;,
         0.000000; 0.000000; 1.000000;,
         0.000000; 0.000000; 1.000000;;
        1;
        4;3,2,1,0;;
      } // End of TopWater normals
      MeshTextureCoords { // TopWater UV coordinates
        4;
         0.000000; 1.000000;,
         1.000000; 1.000000;,
         1.000000; 0.000000;,
         0.000000; 0.000000;;
      } // End of TopWater UV coordinates
      MeshMaterialList { // TopWater material list
        1;
        1;
        0;
        Material Material_007 {
           0.640000; 0.640000; 0.640000; 0.431818;;
           249.019608;
           0.400000; 0.400000; 0.400000;;
           0.000000; 0.000000; 0.000000;;
          TextureFilename {"TopWaterLayerTexture.png";}
        }
      } // End of TopWater material list
    } // End of TopWater mesh
  } // End of TopWater
} // End of Root