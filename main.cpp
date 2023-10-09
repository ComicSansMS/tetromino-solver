#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <functional>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <DLX.hpp>
#include <problem_instance.hpp>
#include <tetromino.hpp>

void printField(Polyomino::FieldSize dim, int x, int y, Tetromino::OneSided::Placement const& placement)
{
    for(int iy = 0; iy < dim.y; ++iy)
    {
        for(int ix = 0; ix < dim.x; ++ix)
        {
            bool is_occupied = false;
            for(auto const& p : placement.layout)
            {
                if(x + p.x == ix && y + p.y == iy) { is_occupied = true; }
            }
            std::cout << (is_occupied ? "X" : ".");
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void enumerateSample()
{
    Polyomino::FieldSize field{ 5, 6 };
    for(auto s = Tetromino::OneSided::begin(); s < Tetromino::OneSided::end(); s = inc(s))
    {
        for(int rot = 0; rot < getRotations(s); ++ rot)
        {
            auto const placement = Tetromino::OneSided::getPlacement(s, rot);
            for(int x = 0; x < (field.x - placement.bound.x + 1); ++x)
            {
                for(int y = 0; y < (field.y - placement.bound.y + 1); ++y)
                {
                    printField(field, x, y, placement);
                }
            }
        }
    }
}

template<typename Shape_T>
void printShape(std::ostream& os, void const* s)
{
    os << "Piece " << *reinterpret_cast<Shape_T const*>(s) << " - ";
}

template<typename Shape_T>
void printSolution(DLX::Matrix::Solution const& solution,
                   Polyomino::ProblemInstance<Shape_T> const& problem, DLX::Matrix const& m)
{
    for(auto const& row : solution)
    {
        m.printRow(row, std::cout, problem.getCurrentPieceCount(),
                   problem.getFieldSize().x, printShape<Shape_T>, false);
        std::cout << "\n\n";
    }
}

template<typename Shape_T>
void printSolutions(std::vector<DLX::Matrix::Solution> const& solutions,
                    Polyomino::ProblemInstance<Shape_T> const& problem, DLX::Matrix const& m)
{
    for(std::size_t i = 0; i < solutions.size(); ++i)
    {
        std::cout << "\n *** Solution #" << i+1 << "/" << solutions.size() << ": ***\n" << std::endl;
        auto const& solution = solutions[i];
        printSolution(solution, problem, m);
    }
}

template<typename Shape_T>
void solveProblem(Polyomino::ProblemInstance<Shape_T> const& problem, bool computeAllSolutions = true)
{
    DLX::Matrix m = problem.calculateProblemMatrix();
    if (computeAllSolutions) {
        m.printMatrix(std::cout, problem.getCurrentPieceCount(), problem.getFieldSize().x, printShape<Shape_T>, true);
        auto solutions = m.solveAll();
        printSolutions(solutions, problem, m);
    } else {
        printSolution(m.solve(), problem, m);
    }

}

void green1()
{
    using namespace Tetromino::OneSided;
    Polyomino::ProblemInstance<Shape> problem(Polyomino::FieldSize{4, 3});
    problem.addPiece(Shape::J);
    problem.addPiece(Shape::J);
    problem.addPiece(Shape::Z);

    solveProblem(problem);
}

void blueA3()
{
    using namespace Tetromino::OneSided;
    Polyomino::ProblemInstance<Shape> problem(Polyomino::FieldSize{6, 6});
    problem.addPiece(Shape::T);
    problem.addPiece(Shape::T);
    problem.addPiece(Shape::O);
    problem.addPiece(Shape::O);
    problem.addPiece(Shape::I);
    problem.addPiece(Shape::J);
    problem.addPiece(Shape::L);
    problem.addPiece(Shape::L);
    problem.addPiece(Shape::S);

    solveProblem(problem);
}

void buildProblemFromString(int field_width, int field_height, std::string const& pieces)
{
    if((field_width * field_height) != pieces.length() * 4) {
        std::cout << "Not enough pieces to fill the field" << std::endl;
        return;
    }

    using namespace Tetromino::OneSided;
    Polyomino::ProblemInstance<Shape> problem(Polyomino::FieldSize{field_width, field_height});
    for(char c : pieces)
    {
        switch(c)
        {
        case 'i': case 'I': problem.addPiece(Shape::I); break;
        case 'o': case 'O': problem.addPiece(Shape::O); break;
        case 't': case 'T': problem.addPiece(Shape::T); break;
        case 'j': case 'J': problem.addPiece(Shape::J); break;
        case 'l': case 'L': problem.addPiece(Shape::L); break;
        case 's': case 'S': problem.addPiece(Shape::S); break;
        case 'z': case 'Z': problem.addPiece(Shape::Z); break;
        default: std::cout << "Unknown shape \'" << c << "\'" << std::endl; return;
        }
    }
    solveProblem(problem, false);
}

void readProblemFromFile(std::string const& filename)
{
    std::ifstream fin(filename);
    if(!fin) {
        std::cerr << "File could not be opened: " << filename << std::endl;
        std::exit(1);
    }

    int field_width, field_height;
    fin >> field_width;
    fin >> field_height;

    std::string pieces;
    fin >> pieces;
    buildProblemFromString(field_width, field_height, pieces);
}

int main(int argc, char* argv[])
{
    //*
    if(argc != 2 && argc != 4)
    {
        std::cout << "Usage: \n"
                  << "  tetromino_solver w h IOTJLSZ\n"
                  << " or\n"
                  << "  tetromino_solver <filename>\n"
                  << std::endl;
    } else if(argc == 2)
    {
        readProblemFromFile(argv[1]);
    } else if(argc == 4)
    {
        buildProblemFromString( std::atoi(argv[1]), std::atoi(argv[2]), argv[3]);
    }
    /*/
    if(argc != 2) { std::cout << "No file." << std::endl; return 1; }
    std::fstream fin(argv[1]);
    std::string tmp;
    std::vector<std::string> mat_str;
    while (std::getline(fin, tmp)) {
        mat_str.push_back(tmp);
    }
    int const n_cols = static_cast<int>(mat_str.front().length());
    if(!std::all_of(begin(mat_str), end(mat_str), [=](std::string const& s) { return s.length() == n_cols; }))
    {
        return 1;
    }
    std::cout << "Problem matrix " << mat_str.size() << "x" << n_cols << "." << std::endl;

    DLX::Matrix m(n_cols);
    for(auto const& row_str : mat_str)
    {
        DLX::RowHeader rh;
        rh.UserData = &row_str;
        std::vector<int> fields;
        for(int i=0; i<row_str.size(); ++i)
        {
            if(row_str[i] == '1') {
                fields.push_back(i);
            } else if(row_str[i] != '0') {
                return 1;
            }
        }
        m.addRow(rh, fields);
    }

    m.printMatrix(std::cout, n_cols, 0, DLX::RowHeaderUserDataPrinter(), true);


    std::cout << "Calculating solution..." << std::endl;
    auto const t_start = std::chrono::steady_clock::now();
    auto solutions = m.solveAll();
    auto const t_end = std::chrono::steady_clock::now();

    std::cout << "Found " << solutions.size() << " solutions:" << std::endl;
    int i = 0;
    for(auto const solution : solutions)
    {
        std::cout << "Solution #" << ++i << '\n';
        for(auto const& s : solution)
        {
            std::cout << mat_str[s] << '\n';
        }
        std::cout << '\n';
    }
    std::cout << '\n' << std::endl;

    std::cout << "Compute time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count() << "ms."
              << std::endl;

    */
}
