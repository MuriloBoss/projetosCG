import { useState, useRef, useEffect } from "react";

const COLORS = {
  bg: "#0f1117",
  card: "#181c27",
  cardBorder: "#252a3a",
  accent: "#4f8ef7",
  accent2: "#a78bfa",
  accent3: "#34d399",
  accent4: "#f59e0b",
  text: "#e8eaf0",
  textMuted: "#7a82a0",
  gridLine: "#1e2235",
  axisLine: "#2e3450",
  original: "#4f8ef7",
  transformed: "#f43f5e",
  point: "#34d399",
};

function useFont() {
  useEffect(() => {
    const link = document.createElement("link");
    link.rel = "stylesheet";
    link.href = "https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;700&family=Syne:wght@400;600;800&display=swap";
    document.head.appendChild(link);
  }, []);
}

// ---- MATH ----
function matMul(A, B) {
  const R = [[0,0,0],[0,0,0],[0,0,0]];
  for (let i=0;i<3;i++) for (let j=0;j<3;j++) for (let k=0;k<3;k++) R[i][j]+=A[i][k]*B[k][j];
  return R;
}
function applyMat(M, pts) {
  return pts.map(([x,y]) => {
    const nx = M[0][0]*x + M[0][1]*y + M[0][2];
    const ny = M[1][0]*x + M[1][1]*y + M[1][2];
    return [nx, ny];
  });
}
function T(tx,ty) { return [[1,0,tx],[0,1,ty],[0,0,1]]; }
function S(sx,sy) { return [[sx,0,0],[0,sy,0],[0,0,1]]; }
function R(deg) {
  const r = deg*Math.PI/180;
  return [[Math.cos(r),-Math.sin(r),0],[Math.sin(r),Math.cos(r),0],[0,0,1]];
}
function Rx() { return [[1,0,0],[0,-1,0],[0,0,1]]; }
function Ry() { return [[-1,0,0],[0,1,0],[0,0,1]]; }
function Sh(a) { return [[1,a,0],[0,1,0],[0,0,1]]; }
function Sv(b) { return [[1,0,0],[b,1,0],[0,0,1]]; }

function fmt(n) {
  const v = Math.abs(n) < 1e-10 ? 0 : n;
  return v.toFixed(2).replace(/\.?0+$/, '') || '0';
}

function MatrixDisplay({ M, label, color = COLORS.accent }) {
  return (
    <div style={{ display:'flex', flexDirection:'column', alignItems:'center', margin:'0 6px' }}>
      {label && <div style={{ fontSize:11, color:COLORS.textMuted, marginBottom:4, fontFamily:"'Syne',sans-serif", textTransform:'uppercase', letterSpacing:1 }}>{label}</div>}
      <div style={{
        border:`1.5px solid ${color}44`,
        borderRadius:8,
        padding:'8px 12px',
        background:`${color}08`,
        fontFamily:"'JetBrains Mono',monospace",
        fontSize:12,
        color: COLORS.text,
        display:'grid',
        gridTemplateColumns:'repeat(3,1fr)',
        gap:'2px 14px',
        minWidth:130,
      }}>
        {M.flat().map((v,i) => (
          <span key={i} style={{ textAlign:'right', color: Math.abs(v)<1e-10 ? COLORS.textMuted : COLORS.text }}>
            {fmt(v)}
          </span>
        ))}
      </div>
    </div>
  );
}

// ---- CANVAS GRID ----
function Grid({ original, transformed, pivot, range=12, size=320 }) {
  const canvasRef = useRef();
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    ctx.clearRect(0,0,size,size);
    const scale = size/(2*range);
    const cx = size/2, cy = size/2;
    const toCanvas = ([x,y]) => [cx+x*scale, cy-y*scale];

    // grid
    ctx.strokeStyle = COLORS.gridLine;
    ctx.lineWidth = 0.5;
    for (let i=-range;i<=range;i++) {
      ctx.beginPath(); ctx.moveTo(cx+i*scale,0); ctx.lineTo(cx+i*scale,size); ctx.stroke();
      ctx.beginPath(); ctx.moveTo(0,cy+i*scale); ctx.lineTo(size,cy+i*scale); ctx.stroke();
    }
    // axes
    ctx.strokeStyle = COLORS.axisLine;
    ctx.lineWidth = 1.5;
    ctx.beginPath(); ctx.moveTo(0,cy); ctx.lineTo(size,cy); ctx.stroke();
    ctx.beginPath(); ctx.moveTo(cx,0); ctx.lineTo(cx,size); ctx.stroke();
    // axis labels
    ctx.fillStyle = COLORS.textMuted;
    ctx.font = "11px 'JetBrains Mono'";
    for (let i=-range+2;i<=range-2;i+=2) {
      if (i!==0) {
        ctx.fillText(i, cx+i*scale-6, cy+14);
        ctx.fillText(i, cx+4, cy-i*scale+4);
      }
    }

    const drawPoly = (pts, color, fill, label) => {
      if (!pts || pts.length < 2) return;
      const cpts = pts.map(toCanvas);
      ctx.beginPath();
      ctx.moveTo(...cpts[0]);
      for (let i=1;i<cpts.length;i++) ctx.lineTo(...cpts[i]);
      ctx.closePath();
      if (fill) { ctx.fillStyle = color+'22'; ctx.fill(); }
      ctx.strokeStyle = color;
      ctx.lineWidth = 2;
      ctx.stroke();
      cpts.forEach(([px,py], i) => {
        ctx.beginPath();
        ctx.arc(px, py, 4, 0, Math.PI*2);
        ctx.fillStyle = color;
        ctx.fill();
        ctx.fillStyle = color;
        ctx.font = "bold 11px 'JetBrains Mono'";
        ctx.fillText(label ? `${label[i]}` : '', px+6, py-4);
      });
    };

    drawPoly(original, COLORS.original, true, ['A','B','C']);
    drawPoly(transformed, COLORS.transformed, true, ["A'","B'","C'"]);

    // pivot
    if (pivot) {
      const [px,py] = toCanvas(pivot);
      ctx.beginPath(); ctx.arc(px,py,5,0,Math.PI*2);
      ctx.fillStyle = COLORS.accent4;
      ctx.fill();
      ctx.strokeStyle = '#fff'; ctx.lineWidth=1; ctx.stroke();
    }
  }, [original, transformed, pivot, range, size]);

  return (
    <canvas ref={canvasRef} width={size} height={size}
      style={{ borderRadius:12, background:COLORS.bg, border:`1px solid ${COLORS.cardBorder}` }} />
  );
}

// ---- SECTIONS ----
const TRIANGLE = [[4,2],[8,2],[6,7]];

const TRANSFORMATIONS = [
  {
    id:'translation',
    title:'a) Translação',
    subtitle:'Ty = 3',
    color: COLORS.accent,
    theory: 'Move o objeto somando Tx e Ty às coordenadas. Com coordenadas homogêneas, vira uma multiplicação de matriz.',
    formula: 'T(Tx,Ty)',
    params: [{label:'Tx', key:'tx', min:-8, max:8, step:0.5, default:0},
             {label:'Ty', key:'ty', min:-8, max:8, step:0.5, default:3}],
    compute: ({tx,ty}) => T(tx,ty),
    inverse: 'T⁻¹ = T(-Tx, -Ty)',
    matLabels: (p) => [`T(${p.tx},${p.ty})`],
    mats: (p) => [T(p.tx,p.ty)],
  },
  {
    id:'scale',
    title:'b) Escala',
    subtitle:'Sx=0.5, Sy=0.5',
    color: COLORS.accent2,
    theory: 'Multiplica as coordenadas pelos fatores Sx e Sy. Valores < 1 reduzem, > 1 ampliam.',
    formula: 'S(Sx,Sy)',
    params: [{label:'Sx', key:'sx', min:0.1, max:3, step:0.1, default:0.5},
             {label:'Sy', key:'sy', min:0.1, max:3, step:0.1, default:0.5}],
    compute: ({sx,sy}) => S(sx,sy),
    inverse: 'S⁻¹ = S(1/Sx, 1/Sy)',
    matLabels: (p) => [`S(${p.sx},${p.sy})`],
    mats: (p) => [S(p.sx,p.sy)],
  },
  {
    id:'shearH',
    title:'c) Cisalhamento Horizontal',
    subtitle:'a = 1.2',
    color: COLORS.accent3,
    theory: 'Distorce o objeto horizontalmente. x\' = x + a·y  (y não muda). "Inclina" o objeto.',
    formula: 'Sh(a)',
    params: [{label:'a', key:'a', min:-3, max:3, step:0.1, default:1.2}],
    compute: ({a}) => Sh(a),
    inverse: 'Sh⁻¹ = Sh(-a)',
    matLabels: (p) => [`Sh(${p.a})`],
    mats: (p) => [Sh(p.a)],
  },
  {
    id:'reflX',
    title:'d) Reflexão — Eixo X',
    subtitle:'Espelha em torno de y=0',
    color: COLORS.accent4,
    theory: 'Inverte o sinal de y. O objeto é espelhado em relação ao eixo X.',
    formula: 'Rx',
    params: [],
    compute: () => Rx(),
    inverse: 'Rx⁻¹ = Rx (própria inversa)',
    matLabels: () => ['Rx'],
    mats: () => [Rx()],
  },
  {
    id:'rotation',
    title:'e) Rotação',
    subtitle:'θ = -30°',
    color: '#f43f5e',
    theory: 'Rotaciona em torno da origem. Ângulo positivo = anti-horário, negativo = horário.',
    formula: 'R(θ)',
    params: [{label:'θ (graus)', key:'deg', min:-180, max:180, step:5, default:-30}],
    compute: ({deg}) => R(deg),
    inverse: 'R⁻¹ = R(-θ) = Rᵀ',
    matLabels: (p) => [`R(${p.deg}°)`],
    mats: (p) => [R(p.deg)],
  },
  {
    id:'rotPivot',
    title:'f) Rotação em torno de (6,4)',
    subtitle:'θ = -30°, ponto (6,4)',
    color: '#fb923c',
    theory: 'Para rotar em torno de um ponto P(px,py): 1) Translada P para origem  2) Rotaciona  3) Translada de volta. Matriz: T·R·T⁻¹',
    formula: 'T(px,py) · R(θ) · T(-px,-py)',
    params: [{label:'θ (graus)', key:'deg', min:-180, max:180, step:5, default:-30},
             {label:'px', key:'px', min:-8, max:8, step:0.5, default:6},
             {label:'py', key:'py', min:-8, max:8, step:0.5, default:4}],
    compute: ({deg,px,py}) => matMul(matMul(T(px,py), R(deg)), T(-px,-py)),
    inverse: null,
    matLabels: (p) => [`T(${p.px},${p.py})`, `R(${p.deg}°)`, `T(${-p.px},${-p.py})`],
    mats: (p) => [T(p.px,p.py), R(p.deg), T(-p.px,-p.py)],
    pivot: (p) => [p.px, p.py],
  },
  {
    id:'scalePivot',
    title:'g) Escala em torno de (6,4)',
    subtitle:'Sx=0.5, Sy=0.5, ponto (6,4)',
    color: '#22d3ee',
    theory: 'Mesmo princípio da rotação com pivô: T·S·T⁻¹. Escala em relação a um ponto fixo.',
    formula: 'T(px,py) · S(Sx,Sy) · T(-px,-py)',
    params: [{label:'Sx', key:'sx', min:0.1, max:3, step:0.1, default:0.5},
             {label:'Sy', key:'sy', min:0.1, max:3, step:0.1, default:0.5},
             {label:'px', key:'px', min:-8, max:8, step:0.5, default:6},
             {label:'py', key:'py', min:-8, max:8, step:0.5, default:4}],
    compute: ({sx,sy,px,py}) => matMul(matMul(T(px,py), S(sx,sy)), T(-px,-py)),
    matLabels: (p) => [`T(${p.px},${p.py})`, `S(${p.sx},${p.sy})`, `T(${-p.px},${-p.py})`],
    mats: (p) => [T(p.px,p.py), S(p.sx,p.sy), T(-p.px,-p.py)],
    pivot: (p) => [p.px, p.py],
  },
  {
    id:'reflYpivot',
    title:'h) Reflexão Y em torno de (6,4)',
    subtitle:'T(3,-2) + Refl. Y, pivô (6,4)',
    color: '#e879f9',
    theory: 'Composição: T·(Translação·ReflexãoY)·T⁻¹. Múltiplas transformações combinadas em torno de um pivô.',
    formula: 'T(px,py) · T(Tx,Ty) · Ry · T(-px,-py)',
    params: [{label:'Tx', key:'tx', min:-8, max:8, step:0.5, default:3},
             {label:'Ty', key:'ty', min:-8, max:8, step:0.5, default:-2},
             {label:'px', key:'px', min:-8, max:8, step:0.5, default:6},
             {label:'py', key:'py', min:-8, max:8, step:0.5, default:4}],
    compute: ({tx,ty,px,py}) => matMul(matMul(T(px,py), matMul(T(tx,ty), Ry())), T(-px,-py)),
    matLabels: (p) => [`T(${p.px},${p.py})`, `T(${p.tx},${p.ty})`, `Ry`, `T(${-p.px},${-p.py})`],
    mats: (p) => [T(p.px,p.py), T(p.tx,p.ty), Ry(), T(-p.px,-p.py)],
    pivot: (p) => [p.px, p.py],
  },
  {
    id:'composite',
    title:'i) Composição em torno de (6,4)',
    subtitle:'Sv(1.5) + S(0.5,1) + T(2,0)',
    color: '#a3e635',
    theory: 'Composição completa em torno de pivô. A ordem importa! Leia da direita para esquerda na multiplicação.',
    formula: 'T(px,py) · T(2,0) · S(0.5,1) · Sv(1.5) · T(-px,-py)',
    params: [{label:'b', key:'b', min:-3, max:3, step:0.1, default:1.5},
             {label:'Sx', key:'sx', min:0.1, max:3, step:0.1, default:0.5},
             {label:'Sy', key:'sy', min:0.1, max:3, step:0.1, default:1.0},
             {label:'Ttx', key:'ttx', min:-8, max:8, step:0.5, default:2},
             {label:'px', key:'px', min:-8, max:8, step:0.5, default:6},
             {label:'py', key:'py', min:-8, max:8, step:0.5, default:4}],
    compute: ({b,sx,sy,ttx,px,py}) => {
      const inner = matMul(matMul(T(ttx,0), S(sx,sy)), Sv(b));
      return matMul(matMul(T(px,py), inner), T(-px,-py));
    },
    matLabels: (p) => [`T(${p.px},${p.py})`, `T(${p.ttx},0)`, `S(${p.sx},${p.sy})`, `Sv(${p.b})`, `T(${-p.px},${-p.py})`],
    mats: (p) => [T(p.px,p.py), T(p.ttx,0), S(p.sx,p.sy), Sv(p.b), T(-p.px,-p.py)],
    pivot: (p) => [p.px, p.py],
  },
];

// ---- THEORY CARD ----
function TheorySection() {
  const matrices = [
    { label: 'Translação', color: COLORS.accent, mat: [[1,0,'Tx'],[0,1,'Ty'],[0,0,1]], inv: 'T(-Tx,-Ty)' },
    { label: 'Escala', color: COLORS.accent2, mat: [['Sx',0,0],[0,'Sy',0],[0,0,1]], inv: 'S(1/Sx,1/Sy)' },
    { label: 'Rotação', color: '#f43f5e', mat: [['cosθ','-sinθ',0],['sinθ','cosθ',0],[0,0,1]], inv: 'R(-θ) = Rᵀ' },
    { label: 'Refl. Eixo X', color: COLORS.accent4, mat: [[1,0,0],[0,-1,0],[0,0,1]], inv: 'Própria inversa' },
    { label: 'Refl. Eixo Y', color: '#e879f9', mat: [[-1,0,0],[0,1,0],[0,0,1]], inv: 'Própria inversa' },
    { label: 'Cis. Horiz.', color: COLORS.accent3, mat: [[1,'a',0],[0,1,0],[0,0,1]], inv: 'Sh(-a)' },
    { label: 'Cis. Vert.', color: '#a3e635', mat: [[1,0,0],['b',1,0],[0,0,1]], inv: 'Sv(-b)' },
  ];

  return (
    <div style={{ marginBottom:32 }}>
      <div style={{ fontFamily:"'Syne',sans-serif", fontSize:22, fontWeight:800, color:COLORS.text, marginBottom:4 }}>
        📐 Matrizes de Transformação
      </div>
      <div style={{ color:COLORS.textMuted, fontSize:13, marginBottom:20, fontFamily:"'JetBrains Mono',monospace" }}>
        Todo ponto é representado como (x, y, 1) — coordenadas homogêneas.
      </div>
      <div style={{ display:'flex', flexWrap:'wrap', gap:12 }}>
        {matrices.map(({label,color,mat,inv}) => (
          <div key={label} style={{
            background: COLORS.card,
            border:`1px solid ${color}33`,
            borderRadius:12,
            padding:'14px 16px',
            minWidth:170,
            flex:'1 1 160px',
          }}>
            <div style={{ color, fontFamily:"'Syne',sans-serif", fontWeight:700, fontSize:13, marginBottom:8 }}>{label}</div>
            <div style={{
              display:'grid', gridTemplateColumns:'repeat(3,1fr)', gap:'2px 10px',
              fontFamily:"'JetBrains Mono',monospace", fontSize:12, color:COLORS.text,
              marginBottom:8,
            }}>
              {mat.flat().map((v,i) => (
                <span key={i} style={{ textAlign:'right',
                  color: v===0 ? COLORS.textMuted : typeof v === 'string' ? color : COLORS.text
                }}>{v}</span>
              ))}
            </div>
            <div style={{ fontSize:10, color:COLORS.textMuted, fontFamily:"'JetBrains Mono',monospace" }}>
              inv: {inv}
            </div>
          </div>
        ))}
      </div>

      <div style={{
        marginTop:20,
        background:`${COLORS.accent4}11`,
        border:`1px solid ${COLORS.accent4}33`,
        borderRadius:12, padding:'14px 18px',
      }}>
        <div style={{ color:COLORS.accent4, fontFamily:"'Syne',sans-serif", fontWeight:700, fontSize:14, marginBottom:6 }}>
          ⚠️ Transformação em torno de um ponto arbitrário (px, py)
        </div>
        <div style={{ color:COLORS.text, fontFamily:"'JetBrains Mono',monospace", fontSize:13 }}>
          M = T(px,py) · (transformação) · T(-px,-py)
        </div>
        <div style={{ color:COLORS.textMuted, fontSize:12, marginTop:6, fontFamily:"'JetBrains Mono',monospace" }}>
          1. Translada pivô para origem  →  2. Aplica transformação  →  3. Translada de volta
        </div>
      </div>

      <div style={{
        marginTop:12,
        background:`${COLORS.accent2}11`,
        border:`1px solid ${COLORS.accent2}33`,
        borderRadius:12, padding:'14px 18px',
      }}>
        <div style={{ color:COLORS.accent2, fontFamily:"'Syne',sans-serif", fontWeight:700, fontSize:14, marginBottom:6 }}>
          🔗 Composição de transformações
        </div>
        <div style={{ color:COLORS.text, fontFamily:"'JetBrains Mono',monospace", fontSize:13 }}>
          P' = (Mn · ... · M2 · M1) · P
        </div>
        <div style={{ color:COLORS.textMuted, fontSize:12, marginTop:6, fontFamily:"'JetBrains Mono',monospace" }}>
          A ordem importa! Multiplica matrizes da direita para esquerda, aplica no ponto por último.
        </div>
      </div>
    </div>
  );
}

// ---- TRANSFORMATION CARD ----
function TransformCard({ tf }) {
  const defaultParams = {};
  tf.params.forEach(p => { defaultParams[p.key] = p.default; });
  const [params, setParams] = useState(defaultParams);

  const M = tf.compute(params);
  const transformed = applyMat(M, TRIANGLE);
  const pivot = tf.pivot ? tf.pivot(params) : null;
  const mats = tf.mats(params);
  const matLabels = tf.matLabels(params);
  const finalMat = mats.reduce((acc,m) => matMul(acc,m), [[1,0,0],[0,1,0],[0,0,1]]);

  return (
    <div style={{
      background: COLORS.card,
      border:`1px solid ${tf.color}33`,
      borderRadius:16,
      padding:'20px 22px',
      marginBottom:18,
    }}>
      <div style={{ display:'flex', alignItems:'flex-start', gap:10, marginBottom:14 }}>
        <div style={{
          background:`${tf.color}22`, color:tf.color,
          borderRadius:8, padding:'4px 12px',
          fontFamily:"'Syne',sans-serif", fontWeight:800, fontSize:15,
        }}>{tf.title}</div>
        <div style={{ color:COLORS.textMuted, fontSize:12, fontFamily:"'JetBrains Mono',monospace", paddingTop:6 }}>
          {tf.subtitle}
        </div>
      </div>

      <div style={{ color:COLORS.textMuted, fontSize:13, marginBottom:14, fontFamily:"'JetBrains Mono',monospace",
        background:`${COLORS.bg}88`, borderRadius:8, padding:'8px 12px' }}>
        💡 {tf.theory}
      </div>

      <div style={{ display:'flex', flexWrap:'wrap', gap:16, alignItems:'flex-start' }}>
        {/* sliders */}
        {tf.params.length > 0 && (
          <div style={{ minWidth:180, flex:'0 0 auto' }}>
            {tf.params.map(p => (
              <div key={p.key} style={{ marginBottom:10 }}>
                <div style={{ display:'flex', justifyContent:'space-between', marginBottom:3 }}>
                  <span style={{ color:tf.color, fontFamily:"'JetBrains Mono',monospace", fontSize:12 }}>{p.label}</span>
                  <span style={{ color:COLORS.text, fontFamily:"'JetBrains Mono',monospace", fontSize:12, fontWeight:'bold' }}>
                    {fmt(params[p.key])}
                  </span>
                </div>
                <input type="range" min={p.min} max={p.max} step={p.step} value={params[p.key]}
                  onChange={e => setParams(prev => ({...prev, [p.key]: parseFloat(e.target.value)}))}
                  style={{ width:'100%', accentColor: tf.color }} />
              </div>
            ))}
          </div>
        )}

        {/* grid */}
        <Grid original={TRIANGLE} transformed={transformed} pivot={pivot} range={14} size={260} />

        {/* matrix decomposition */}
        <div style={{ flex:1, minWidth:200 }}>
          <div style={{ color:COLORS.textMuted, fontSize:11, fontFamily:"'Syne',sans-serif", textTransform:'uppercase', letterSpacing:1, marginBottom:8 }}>
            Composição de matrizes
          </div>
          <div style={{ display:'flex', flexWrap:'wrap', alignItems:'center', gap:4, marginBottom:14 }}>
            {mats.map((m,i) => (
              <div key={i} style={{ display:'flex', alignItems:'center', gap:4 }}>
                <MatrixDisplay M={m} label={matLabels[i]} color={tf.color} />
                {i < mats.length-1 && <span style={{ color:COLORS.textMuted, fontSize:18 }}>·</span>}
              </div>
            ))}
          </div>

          <div style={{ color:COLORS.textMuted, fontSize:11, fontFamily:"'Syne',sans-serif", textTransform:'uppercase', letterSpacing:1, marginBottom:6, marginTop:8 }}>
            Matriz resultante M
          </div>
          <MatrixDisplay M={finalMat} color={tf.color} />

          <div style={{ marginTop:14 }}>
            <div style={{ color:COLORS.textMuted, fontSize:11, fontFamily:"'Syne',sans-serif", textTransform:'uppercase', letterSpacing:1, marginBottom:4 }}>
              Pontos transformados
            </div>
            <div style={{ display:'flex', gap:10, flexWrap:'wrap' }}>
              {['A','B','C'].map((l,i) => (
                <div key={l} style={{
                  background:`${tf.color}11`, border:`1px solid ${tf.color}33`,
                  borderRadius:8, padding:'6px 10px',
                  fontFamily:"'JetBrains Mono',monospace", fontSize:11,
                }}>
                  <span style={{ color:tf.color }}>{l}'</span>
                  <span style={{ color:COLORS.textMuted }}> = </span>
                  <span style={{ color:COLORS.text }}>({fmt(transformed[i][0])}, {fmt(transformed[i][1])})</span>
                </div>
              ))}
            </div>
          </div>

          {tf.inverse && (
            <div style={{ marginTop:10, fontSize:11, color:COLORS.textMuted, fontFamily:"'JetBrains Mono',monospace" }}>
              🔄 {tf.inverse}
            </div>
          )}
        </div>
      </div>
    </div>
  );
}

// ---- LEGEND ----
function Legend() {
  return (
    <div style={{ display:'flex', gap:16, marginBottom:20, flexWrap:'wrap' }}>
      {[
        [COLORS.original, 'Original A(4,2) B(8,2) C(6,7)'],
        [COLORS.transformed, "Transformado A' B' C'"],
        [COLORS.accent4, 'Ponto pivô (quando aplicável)'],
      ].map(([color,label]) => (
        <div key={label} style={{ display:'flex', alignItems:'center', gap:6 }}>
          <div style={{ width:14, height:14, borderRadius:3, background:color }} />
          <span style={{ color:COLORS.textMuted, fontSize:12, fontFamily:"'JetBrains Mono',monospace" }}>{label}</span>
        </div>
      ))}
    </div>
  );
}

export default function App() {
  useFont();
  const [tab, setTab] = useState('teoria');

  return (
    <div style={{
      background: COLORS.bg,
      minHeight: '100vh',
      padding: '28px 20px',
      boxSizing: 'border-box',
      maxWidth: 1100,
      margin: '0 auto',
      fontFamily: "'Syne', sans-serif",
      color: COLORS.text,
    }}>
      <div style={{ marginBottom:28 }}>
        <div style={{
          fontSize:11, color:COLORS.accent, fontFamily:"'JetBrains Mono',monospace",
          letterSpacing:3, textTransform:'uppercase', marginBottom:6
        }}>Computação Gráfica — IFSUL</div>
        <h1 style={{ margin:0, fontSize:32, fontWeight:800, color:COLORS.text, lineHeight:1.1 }}>
          Transformações Geométricas 2D
        </h1>
        <div style={{ color:COLORS.textMuted, fontSize:14, marginTop:6, fontFamily:"'JetBrains Mono',monospace" }}>
          Material interativo · Lista 3 · Triângulo A(4,2) B(8,2) C(6,7)
        </div>
      </div>

      {/* tabs */}
      <div style={{ display:'flex', gap:8, marginBottom:24, borderBottom:`1px solid ${COLORS.cardBorder}`, paddingBottom:12 }}>
        {[['teoria','📖 Teoria'], ['exercicios','🧮 Exercícios']].map(([id,label]) => (
          <button key={id} onClick={() => setTab(id)} style={{
            background: tab===id ? `${COLORS.accent}22` : 'transparent',
            border: `1px solid ${tab===id ? COLORS.accent : COLORS.cardBorder}`,
            color: tab===id ? COLORS.accent : COLORS.textMuted,
            borderRadius:8, padding:'7px 18px',
            fontFamily:"'Syne',sans-serif", fontWeight:600, fontSize:14,
            cursor:'pointer', transition:'all 0.2s',
          }}>{label}</button>
        ))}
      </div>

      {tab === 'teoria' && <TheorySection />}
      {tab === 'exercicios' && (
        <div>
          <Legend />
          {TRANSFORMATIONS.map(tf => (
            <TransformCard key={tf.id} tf={tf} />
          ))}
        </div>
      )}
    </div>
  );
}
