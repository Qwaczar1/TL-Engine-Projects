xof 0303txt 0032

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000,-0.000000, 1.000000, 0.000000,
     0.000000, 1.000000, 0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame BottomWater {
    FrameTransformMatrix {
      -0.000062,-378.260315, 0.000000, 0.000000,
      378.260315,-0.000062,-0.000017, 0.000000,
       0.000017,-0.000000,378.260315, 0.000000,
       0.000015, 0.000004, 0.000000, 1.000000;;
    }
    Mesh { // BottomWater mesh
      4;
      -10.536143;-11.155689;-0.030163;,
      10.963412;-11.155689;-0.030163;,
      10.963412;10.343866;-0.030163;,
      -10.536143;10.343866;-0.030163;;
      1;
      4;3,2,1,0;;
      MeshNormals { // BottomWater normals
        4;
         0.000000; 0.000000; 1.000000;,
         0.000000; 0.000000; 1.000000;,
         0.000000; 0.000000; 1.000000;,
         0.000000; 0.000000; 1.000000;;
        1;
        4;3,2,1,0;;
      } // End of BottomWater normals
      MeshTextureCoords { // BottomWater UV coordinates
        4;
         0.000000; 1.000000;,
         1.000000; 1.000000;,
         1.000000; 0.000000;,
         0.000000; 0.000000;;
      } // End of BottomWater UV coordinates
      MeshMaterialList { // BottomWater material list
        1;
        1;
        0;
        Material Material_008 {
           0.640000; 0.640000; 0.640000; 1.000000;;
           249.019608;
           0.400000; 0.400000; 0.400000;;
           0.000000; 0.000000; 0.000000;;
          TextureFilename {"BottomWaterLayerTexture.png";}
        }
      } // End of BottomWater material list
    } // End of BottomWater mesh
  } // End of BottomWater
} // End of Root
