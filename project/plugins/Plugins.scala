import sbt._

class Plugins(info: ProjectInfo) extends PluginDefinition(info) {
  val scctRepo = "scct-repo" at "http://mtkopone.github.com/scct/maven-repo/"
  lazy val scctPlugin = "reaktor" % "sbt-scct-for-2.8" % "0.1-SNAPSHOT"

  val riReleases = "RI Releases" at "http://maven.research-infrastructures.eu/nexus/content/repositories/releases"

  val cxDoccoPlugin = "com.github.philcali" % "sbt-cx-docco" % "0.0.1"

  val jcoffescript = "org.jcoffeescript" % "jcoffeescript" % "1.1" from "http://cloud.github.com/downloads/yeungda/jcoffeescript/jcoffeescript-1.1.jar"

  val coffeeScriptSbtRepo = "coffeeScript sbt repo" at "http://repo.coderlukes.com"
  val coffeeScript = "org.coffeescript" % "coffee-script-sbt-plugin" % "1.0"

}			
	
