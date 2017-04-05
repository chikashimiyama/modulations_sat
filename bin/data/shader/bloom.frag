uniform sampler2DRect baseTex;
uniform vec2 bloomSpread;
uniform float bloomGain;

void main() {    
    vec4 coords = gl_TexCoord[0];
    vec4 baseColor = texture2DRect(baseTex, coords.xy);
    vec4 applyColor;
    float cg = bloomGain;
    for (int i =0; i< 4 ;i++){
        vec2 targetCoord = coords.xy;
        targetCoord.x = coords.x + float(i)*bloomSpread.x;
        applyColor = texture2DRect(baseTex, targetCoord.xy);
        targetCoord.x = coords.x - float(i)*bloomSpread.x;
        applyColor += texture2DRect(baseTex, targetCoord.xy);
        baseColor += applyColor * cg;
    }
    cg = bloomGain;
    for (int i =0; i< 4 ;i++){
        vec2 targetCoord = coords.xy;
        targetCoord.y = coords.y + float(i)*bloomSpread.y;
        applyColor = texture2DRect(baseTex, targetCoord.xy);
        targetCoord.y = coords.y - float(i)*bloomSpread.y;
        applyColor += texture2DRect(baseTex, targetCoord.xy);
        baseColor += applyColor * cg;
    }
    gl_FragColor = baseColor;
}