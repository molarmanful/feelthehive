import $ivy.`com.lihaoyi::mill-contrib-scalapblib:`
import contrib.scalapblib._
import mill._
import scala.util.chaining._
import scalalib._
import scalalib.publish._

object main extends ScalaPBModule {

  def scalaVersion  = "3.2.0"
  def scalaPBVersion = "0.11.12"
  def scalaPBGrpc = true
  def scalacOptions = Seq("-deprecation", "-feature")
  // def ivyDeps = Agg(
  //   ivy""
  // )

  object test extends Tests with TestModule.Munit {

    def ivyDeps = Agg(
      ivy"org.scalameta::munit:1.0.0-M6"
    )

  }

}
