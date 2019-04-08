cbuffer cbFixed
{
	float	gQuadIndices[ 6 ] = { 0, 1, 2, 0, 2, 3 };
	float	gIndicesPerQuad = 3;
};

cbuffer cbPerObject
{
	float4x4 gWVP;
};

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;	
};

struct VS_INOUT
{
	float2 	posL		: POSITION;
	float2	scale		: SCALE;
	float	rotation	: ROTATION;
	float4	color		: COLOR;
};

struct PS_IN
{
	float4	posH		: SV_POSITION;
	float4	color		: COLOR;
};

float4 MakePoint( float2 center, float offsetX, float offsetY, float rotation )
{
	float2 fPoint = float2( offsetX, offsetY );
	float4 fReturn = float4( 0.0f, 0.0f, 0.0f, 1.0f );

	fReturn.x = fPoint.x * cos( rotation ) - fPoint.y * sin( rotation );
	fReturn.y = fPoint.y * cos( rotation ) + fPoint.x * sin( rotation );

	fReturn.x += center.x;
	fReturn.y += center.y;

	return ( fReturn );
}


VS_INOUT VS( VS_INOUT vIn )
{
	return ( vIn );
}

[ maxvertexcount( 6 ) ]
void GS( point VS_INOUT input[ 1 ], inout TriangleStream< PS_IN > triStream )
{
	float offsetX = input[ 0 ].scale.x / 2;
	float offsetY = input[ 0 ].scale.y / 2;

	float4 fPoints[] =
	{
		mul( MakePoint( input[ 0 ].posL, -offsetX, offsetY, input[ 0 ].rotation ), gWVP ),
		mul( MakePoint( input[ 0 ].posL, offsetX, offsetY, input[ 0 ].rotation ), gWVP ),
		mul( MakePoint( input[ 0 ].posL, offsetX, -offsetY, input[ 0 ].rotation ), gWVP ),
		mul( MakePoint( input[ 0 ].posL, -offsetX, -offsetY, input[ 0 ].rotation ), gWVP ), 
	};
	
	PS_IN pIn;
	pIn.color = input[ 0 ].color;

	for( int i = 0; i < 2; ++i )
	{
		for( int j = 0; j < gIndicesPerQuad; ++j )
		{
			pIn.posH = fPoints[ gQuadIndices[ i * gIndicesPerQuad + j ] ];
			triStream.Append( pIn );
		}
		triStream.RestartStrip();
	}
}

float4 PS( PS_IN pIn ) : SV_Target
{
	return( pIn.color );
}

technique10 TexTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( CompileShader( gs_4_0, GS() ) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}