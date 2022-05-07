#include "Shader.h"

//#ifndef GL_SILENCE_DEPRECATION
//#define GL_SILENCE_DEPRECATION
//#endif //GL_SILENCE_DEPRECATION

Shader::Shader()
    : shaderProgramID(0)
    , vertexShaderID(0)
    , fragShaderID(0)
{
    
}

Shader::~Shader()
{

}

// シェーダー読み込み
bool Shader::Load(const std::string& vertName, const std::string& fragName){
    
    // シェーダー コンパイル
    if (!CompileShader(vertName, GL_VERTEX_SHADER, vertexShaderID))
    {
        return false;
    }
    
    // コンパイルできてなかったら失敗
    if (!CompileShader(fragName, GL_FRAGMENT_SHADER,fragShaderID))
    {
        return false;
    }
    
    // シェーダー セット
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragShaderID);
    glLinkProgram(shaderProgramID);
    
    // リンクできてなかったら失敗
    if(!IsValidProgram())
    {
        return false;
    }
    
    return true;
}

void Shader::Unload()
{
    // シェーダ削除
    glDeleteProgram(shaderProgramID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragShaderID);
}

// OpenGLにセット
void Shader::SetActive()
{
    // アクティブ化
    glUseProgram(shaderProgramID);
}

// Matrixを送る
void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
    GLuint loc = glGetUniformLocation(shaderProgramID, name);
    glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

// Matrixの配列を送る
void Shader::SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count)
{
    GLuint loc = glGetUniformLocation(shaderProgramID, name);
    glUniformMatrix4fv(loc, count, GL_TRUE, matrices[0].GetAsFloatPtr());
}

// ベクターを送る
void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
    GLuint loc = glGetUniformLocation(shaderProgramID, name);
    glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

// Float配列を送る
void Shader::SetFloatUniform(const char* name, float value)
{
    GLuint loc = glGetUniformLocation(shaderProgramID, name);
    glUniform1f(loc, value);
}


// コンパイル
bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader){
    // シェーダーファイル読み込み
    std::ifstream shaderFile(fileName);
    if (shaderFile.is_open())
    {
        // ソースを読み込む
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        std::string contents = sstream.str();
        const char* contentsChar = contents.c_str();
        
        // シェーダータイプを決める
        outShader = glCreateShader(shaderType);
        // コンパイル
        glShaderSource(outShader, 1, &(contentsChar), nullptr);
        glCompileShader(outShader);
        
        // コンパイルできているか
        if (!IsCompiled(outShader))
        {
            std::cout << "Failed to compile shader:" << fileName.c_str() << "\n" << std::endl;
            return false;
        }
        
    }
    else
    {
        std::cout << "Shader file not found:" << fileName.c_str() << "\n" << std::endl;
        return false;
    }
    
    return true;
}

// コンパイル済みかを確認
bool Shader::IsCompiled(GLuint shader)
{
    GLint status;
    // コンパイルステータス確認
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    
    if (status != GL_TRUE)
    {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetShaderInfoLog(shader, 511, nullptr, buffer);
        std::cout << "GLSL Compile Failed:\n" <<  buffer << "\n" << std::endl;
        return false;
    }
    
    return true;
}

// リンクできているか確認
bool Shader::IsValidProgram()
{
    
    GLint status;
    // リンクステータスを確認
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetProgramInfoLog(shaderProgramID, 511, nullptr, buffer);
        std::cout << "GLSL Link Status:\n%s \n" << std::endl;
        return false;
    }
    
    return true;
}
