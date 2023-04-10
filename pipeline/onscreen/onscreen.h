


const uint max_count = 128;

const uint GLOBAL_MVP_SCENE_BINDINGI   = 0;
const uint GLOBAL_MVP_OVERLAY_BINDINGI = 1; 
const uint GLOBAL_GRIDSTATE_BINDINGI   = 2;
const uint GLOBAL_TEXTITEMS_BINDINGI   = 3;
const uint GLOBAL_FONT_FACE_BINDINGI   = 8; 


layout (push_constant) uniform PushConstant {

  uint resource_id;
  uint _unused_1;
  uint _unused_2;
  uint _unused_3;

  vec4 color;
  vec4 advance;
  vec4 position; 

} pc;

